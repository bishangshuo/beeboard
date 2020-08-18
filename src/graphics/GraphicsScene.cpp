#include "GraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include "src/shape/Rectangle.h"
#include "src/shape/Ellipse.h"
#include "src/shape/Triangle.h"
#include "src/shape/Line.h"
#include "src/shape/Pencil.h"
#include "src/shape/Eraser.h"
#include "src/shape/MultiSelector.h"
#include "src/graphics/GraphicsView.h"
#include "src/shape/PBShapeHelper.h"
#include <QDebug>
#include <QPixmapCache>
#include <QBuffer>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QDataStream>

inline QPixmap GetCursorPixmap(const QString &fileName){
    QPixmap pixmap(fileName);
    return pixmap;
}

inline int seriliazeData(const PBShape::Scene &scene, char *buf,  int size){
    if(buf == NULL){
        return 0;
    }
    size_t total = scene.ByteSizeLong();
    if(size < total){
        return 0;
    }
    if(scene.SerializeToArray(buf, size)){
        return total;
    }
    return 0;
}

GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_eToolType(TOOL_TYPE::SELECT)
    , m_bPressed(false)
    , m_ptPrev()
    , m_nCurKey(0)
    , m_pView(nullptr)
    , m_pSelect(NULL)
    , m_multiSelector(NULL)
    , m_pScenePb(new PBShape::Scene())
{
    QPixmapCache::setCacheLimit(4096000);
    setItemIndexMethod(ItemIndexMethod::NoIndex);
    setBspTreeDepth(4096);

    MakeFileName();
    m_ioSceneObj.moveToThread(&m_thSceneObj);
    connect(this, SIGNAL(sigWriteSceneObject(const char *, int, const QString &)),
            &m_ioSceneObj, SLOT(slotWriteSceneObject(const char *, int, const QString &)));
    connect(&m_ioSceneObj, SIGNAL(sigWriteSceneObjectCompleted(bool, const char *, const QString&)),
            this, SLOT(slotWriteSceneObjectCompleted(bool, const char *, const QString &)));
    m_thSceneObj.start();

}

GraphicsScene::~GraphicsScene(){
    m_thSceneObj.quit();
    m_thSceneObj.wait();

    doClearScene();
    deleteSelectItem();
    destroyMultiSelector();

    m_stRedo.clear();
    m_stUndo.clear();
}

void GraphicsScene::MakeFileName(){
    QDir dir;
    QString dirStr = "files/";
    if(!dir.exists(dirStr)){
        dir.mkdir(dirStr);
    }
    dirStr += "docs/";
    if(!dir.exists(dirStr)){
        dir.mkdir(dirStr);
    }
    m_strSceneObjectFileName = dirStr+QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")+".hd";
}

void GraphicsScene::setToolType(const TOOL_TYPE::Type &toolType){
    m_eToolType = toolType;
}

void GraphicsScene::setView(GraphicsView *view){
    m_pView = view;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sigSceneClicked();
    deleteSelectItem();

    m_bPressed = true;
    QPointF pos = event->scenePos();
    m_ptPrev = pos;

    ShapeBase *shape = nullptr;
    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            QGraphicsScene::mousePressEvent(event);
            if(!onMouseSelectItem(pos)){
                m_pSelect = new Select();
                m_ptSelect = pos;
                m_pSelect->Create(m_ptSelect, m_ptSelect+QPointF(1.0, 1.0), this);
            }
            return;
        }
        case TOOL_TYPE::PENCIL:{
            shape = new Pencil(this);
            Pencil *pencil = dynamic_cast<Pencil *>(shape);
            connect(this, SIGNAL(sigEraserMove(const QPointF &, const QPointF &, Eraser *)),
                    pencil, SLOT(slotEraserMove(const QPointF &, const QPointF &, Eraser *)));
            connect(this, SIGNAL(sigEraserPressed()),
                    pencil, SLOT(slotEraserPressed()));
            connect(this, SIGNAL(sigEraserRelease()),
                    pencil, SLOT(slotEraserRelease()));
            connect(pencil, SIGNAL(sigEraserAttach(int)),
                    this, SLOT(slotEraserAttachToPencil(int)));
            break;
        }
        case TOOL_TYPE::RECTANGLE:{
            shape = new Rectangle(this);
            break;
        }
        case TOOL_TYPE::ELLIPSE:{
            shape = new Ellipse(this);
            break;
        }
        case TOOL_TYPE::TRIANGLE:{
            shape = new Triangle(this);
            break;
        }
        case TOOL_TYPE::LINE:{
            shape = new Line(this);
            break;
        }
        case TOOL_TYPE::ERASER:{
            qDebug() << "going to eraser pencil";
            shape = new Eraser(this);
            Eraser *eraser = dynamic_cast<Eraser *>(shape);
            eraser->setWidth(40);
            emit sigEraserMove(pos, pos, eraser);
            emit sigEraserPressed();
            break;
        }
        case TOOL_TYPE::MOVE:{
            QCursor cur = GetCursorPixmap(":/resources/images/move.png");
            m_pView->setCursor(cur);
            break;
        }
        case TOOL_TYPE::ZOOMIN:{
            QCursor cur = GetCursorPixmap(":/resources/images/zoom-in.png");
            m_pView->setCursor(cur);
            m_pView->zoomIn();
            break;
        }
        case TOOL_TYPE::ZOOMOUT:{
            QCursor cur = GetCursorPixmap(":/resources/images/zoom-out.png");
            m_pView->setCursor(cur);
            m_pView->zoomOut();
            break;
        }
        case TOOL_TYPE::RESET:{
            QCursor cur = GetCursorPixmap(":/resources/images/reset.png");
            m_pView->setCursor(cur);
            m_pView->reset();
            break;
        }
        case TOOL_TYPE::CLEAR:{
            break;
        }
        default:{
            break;
        }
    }

    if(shape != nullptr){
        m_nCurKey = shape->Create(m_ptPrev, m_ptPrev+QPointF(1.0, 1.0), this);
        m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
        //shape->SetSelected(true);
        connect(shape, &ShapeBase::sigRemove, [=](int _key){
            onItemRemove(_key, true);
        });
        connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
            onItemGeoChanged(_key);
        });

        OnSceneChanged();
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if(!m_bPressed){
        return;
    }
    QPointF pos = event->scenePos();

    if(m_pSelect != NULL){
        m_pSelect->UpdateRect(m_ptSelect, pos, this);
        return;
    }

    ShapeBase *shape = nullptr;
    MapShape::iterator it = m_mapShape.find(m_nCurKey);
    if(it != m_mapShape.end()){
        shape = it.value()->shape;
    }
    if(shape == nullptr){
        return;
    }

    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            break;
        }
        case TOOL_TYPE::PENCIL:
            shape->UpdateRect(event->lastScenePos(), pos, this);
            break;
        case TOOL_TYPE::RECTANGLE:
        case TOOL_TYPE::ELLIPSE:
        case TOOL_TYPE::LINE:
        case TOOL_TYPE::TRIANGLE:{
            shape->UpdateRect(m_ptPrev, pos, this);
            break;
        }
        case TOOL_TYPE::ERASER:{
            Eraser *eraser = dynamic_cast<Eraser *>(shape);
            emit sigEraserMove(event->lastScenePos(), event->scenePos(), eraser);
            break;
        }
        case TOOL_TYPE::CLEAR:{
            break;
        }
        default:{
            break;
        }
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if(m_pSelect != NULL){
        QRectF rcSelect = m_pSelect->GetRect();
        SelectItemsByRubberBand(rcSelect);
    }
    if(event->scenePos() == m_ptPrev){
        destroyMultiSelector();
        resetMultiSelector();
    }

    m_bPressed = false;
    ShapeBase *shape = nullptr;
    MapShape::iterator it = m_mapShape.find(m_nCurKey);
    if(it != m_mapShape.end()){
        shape = it.value()->shape;
    }
    if(shape == nullptr){
        return;
    }
    shape->CreateEnd(this);
    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            onSelectedItemsChanged();
            break;
        }
        case TOOL_TYPE::PENCIL:
        case TOOL_TYPE::RECTANGLE:
        case TOOL_TYPE::ELLIPSE:
        case TOOL_TYPE::TRIANGLE:
        case TOOL_TYPE::LINE:{
            SaveItemToProtobuf(shape);
            SaveProtobufToFile();
            //创建新元素，清空所有redo和undo
            destroyRedoShapes();
            m_stUndo.push(ACTION_NODE(act_type_create_item, shape));
            OnSceneChanged();
            break;
        }
        case TOOL_TYPE::ERASER:{
            m_stUndo.push(ACTION_NODE(act_type_erase, shape));
            emit sigEraserRelease();
            break;
        }
        case TOOL_TYPE::MOVE:{
            break;
        }
        case TOOL_TYPE::CLEAR:{
            break;
        }
        default:{
            break;
        }
    }

    if(m_pView != nullptr){
        m_pView->unsetCursor();
    }
}

void GraphicsScene::SetAllEditable(bool editable){
   for(MapShape::iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
       it.value()->shape->SetEditable(editable);
   }
}

void GraphicsScene::onSelectedItemsChanged(){
    bool b = false;
    for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); it++){
        auto itShape = m_mapShape.find(*it);
        if(itShape != m_mapShape.end()){
            b = true;
            ModifyItemInProtobuf(itShape.value()->shape);
            m_stUndo.push(ACTION_NODE(act_type_change_geo, itShape.value()->shape));

        }
    }
    if(b){
        SaveProtobufToFile();
        OnSceneChanged();
    }
}

void GraphicsScene::SelectItemsByRubberBand(const QRectF &rubberBandRect){
//    m_listSelectedItems.clear();
    resetMultiSelector();
    destroyMultiSelector();

    for(MapShape::iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
        QRectF rcShape = it.value()->shape->GetRect();
        if(rcShape.intersects(rubberBandRect) && it.value()->isActivated){
            it.value()->shape->SetSelected(true);
            it.value()->shape->HideControls(true);
            m_listSelectedItems.push_back(it.value()->key);
        }
    }
    if(m_listSelectedItems.size() == 1){
        auto it = m_mapShape.find(m_listSelectedItems.first());
        if(it != m_mapShape.end()){
            it.value()->shape->HideControls(false);
        }
    }else if(m_listSelectedItems.size() > 1){

        //求多选矩形的并集
        QRectF rc;
        for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); it++){
            QRectF rcShape = m_mapShape.find(*it).value()->shape->GetRect();
            rc = rc.united(rcShape);
        }
        //添加选框
        createMultiSelector(rc);
    }else{
        destroyMultiSelector();
    }

    deleteSelectItem();
}

void GraphicsScene::UnselectedAll(){
    for(MapShape::iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
        it.value()->shape->SetSelected(false);
        //it.value()->shape->HideControls(true);
    }
    for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); ){
        it = m_listSelectedItems.erase(it);
    }
    m_listSelectedItems.clear();
}

bool GraphicsScene::onMouseSelectItem(const QPointF &pos)
{
    QPointF itemPos = QPointF(pos.x(), pos.y());
    QGraphicsItem *item = this->itemAt(itemPos, QTransform());
    if(item == nullptr){
        qDebug() << "GraphicsScene::onMouseSelectItem item == nullptr" ;
        deleteSelectItem();
        destroyMultiSelector();

        resetMultiSelector();

        return false;
    }
    int key = item->data(ITEM_DATA_KEY).toInt();
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        it.value()->shape->HideControls(false);
        qDebug() << "content item selected";
    }else{
        qDebug() << "selector item selected, or no items selected";
    }
    return true;
}

void GraphicsScene::resetMultiSelector(){
    for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); it++){
        MapShape::iterator it_shape = m_mapShape.find(*it);
        if(it_shape != m_mapShape.end())
            it_shape.value()->shape->HideControls(false);
    }
    m_listSelectedItems.clear();
}

void GraphicsScene::doClearScene(){
    for(auto it = m_pScenePb->mutable_mapline()->begin(); it != m_pScenePb->mutable_mapline()->end(); ){
        it->second.Clear();
        it = m_pScenePb->mutable_mapline()->erase(it);
    }
    m_pScenePb->mutable_mapline()->clear();

    for(auto it = m_pScenePb->mutable_maprectangle()->begin(); it != m_pScenePb->mutable_maprectangle()->end(); ){
        it->second.Clear();
        it = m_pScenePb->mutable_maprectangle()->erase(it);
    }
    m_pScenePb->mutable_maprectangle()->clear();

    for(auto it = m_pScenePb->mutable_mapellipse()->begin(); it != m_pScenePb->mutable_mapellipse()->end(); ){
        it->second.Clear();
        it = m_pScenePb->mutable_mapellipse()->erase(it);
    }
    m_pScenePb->mutable_mapellipse()->clear();

    for(auto it = m_pScenePb->mutable_maptriangle()->begin(); it != m_pScenePb->mutable_maptriangle()->end(); ){
        it->second.Clear();
        it = m_pScenePb->mutable_maptriangle()->erase(it);
    }
    m_pScenePb->mutable_mapellipse()->clear();

    for(auto it = m_pScenePb->mutable_mapscribble()->begin(); it != m_pScenePb->mutable_mapscribble()->end(); ){
        it->second.Clear();
        it = m_pScenePb->mutable_mapscribble()->erase(it);
    }
    m_pScenePb->mutable_mapscribble()->clear();

    MapShape::iterator it = m_mapShape.begin();
    while(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        data->shape->Remove(this);
        delete shape;
        delete data;
        it = m_mapShape.erase(it);
    }
    m_stUndo.clear();
    m_stRedo.clear();
    m_pView->update();
}

void GraphicsScene::clearScene(){
    doClearScene();
    SaveProtobufToFile();
    OnSceneChanged();
}

void GraphicsScene::destroyRedoShapes(){
    for(auto it = m_stRedo.begin(); it != m_stRedo.end(); ){
        ShapeBase *shape = (*it).shape;
        MapShape::iterator itShape = m_mapShape.find(shape->GetItemKey());
        if(itShape != m_mapShape.end()){
            SHAPE_DATA *data = itShape.value();
            ShapeBase *shapeTemp = itShape.value()->shape;
            shapeTemp->deleteLater();
            delete data;
            itShape = m_mapShape.erase(itShape);
        }
        it = m_stRedo.erase(it);
    }

    m_stRedo.clear();
}

void GraphicsScene::onItemRemove(int key, bool destroy){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        RemoveItemFromProtobuf(shape);
        SaveProtobufToFile();
        //removeItem(shape->GetGraphicsItem());
        shape->Remove(this);
        if(destroy){
            m_stUndo.push(ACTION_NODE(act_type_remove_item, shape));
        }
    }
}

void GraphicsScene::onItemGeoChanged(int key){
    qDebug()<<"GraphicsScene::onItemGeoChanged key="<<key;
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        if(shape->GetType() == TOOL_TYPE::PENCIL ||
                shape->GetType() == TOOL_TYPE::LINE ||
                shape->GetType() == TOOL_TYPE::RECTANGLE ||
                shape->GetType() == TOOL_TYPE::ELLIPSE ||
                shape->GetType() == TOOL_TYPE::TRIANGLE){
            ModifyItemInProtobuf(shape);
            m_stUndo.push(ACTION_NODE(act_type_change_geo, shape));
            SaveProtobufToFile();
            OnSceneChanged();
        }
    }
}

void GraphicsScene::onItemsRemoveByRubberBand(){
    for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); ){
        onItemRemove(*it, true);
        it = m_listSelectedItems.erase(it);
    }
    m_listSelectedItems.clear();
    deleteSelectItem();
    destroyMultiSelector();
}

void GraphicsScene::onMultiSelectedItemsRotate(qreal anchor_x, qreal anchor_y, qreal angle){ 
    QPointF anchorPoint = m_multiSelector->GetRect().center();
    anchorPoint = m_pView->mapFromScene(anchorPoint);
    qDebug() << "GraphicsScene::onMultiSelectedItemsRotate anchor_x="
             << anchor_x << ", anchor_y=" << anchor_y << ", angle=" << angle
             << ", anchor="<<anchorPoint;
    for(ListShapeKey::iterator it_select = m_listSelectedItems.begin(); it_select != m_listSelectedItems.end(); it_select++){
        MapShape::iterator it_shape = m_mapShape.find(*it_select);
        if(it_shape != m_mapShape.end()){
            it_shape.value()->shape->Rotate(anchor_x, anchor_y, angle);
        }
    }
}

void GraphicsScene::deleteSelectItem(){
    if(m_pSelect != NULL){
        m_pSelect->Remove(this);
        m_pSelect->deleteLater();
        m_pSelect = NULL;
    }
}

TOOL_TYPE::Type GraphicsScene::GetPoints(int key, QPoint &p1, QPoint &p2){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return TOOL_TYPE::SELECT;
    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    p1 = shape->GetP1().toPoint();
    p2 = shape->GetP2().toPoint();
    return data->toolType;
}

QPoint GraphicsScene::GetDeltaPos(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        return shape->GetPos().toPoint();
    }
    return QPoint();
}

qreal GraphicsScene::GetAngle(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        return shape->GetAngle();
    }
    return 0.0;
}

void GraphicsScene::createMultiSelector(const QRectF &rc){
    if(m_multiSelector == NULL){
        QRectF rcSelector = QRectF(rc.x()+rc.width()/2, rc.y()+rc.height()/2,
                                   rc.width()+50, rc.height()+50);
        m_multiSelector = new MultiSelector();
        m_multiSelector->Create(rcSelector.topLeft(), rcSelector.bottomRight(), this);
        m_multiSelector->SetSelected(true);

        connect(m_multiSelector, &ShapeBase::sigRemove, [=](int _key){
            onItemsRemoveByRubberBand();
        });

        connect(m_multiSelector, &ShapeBase::sigRotate, [=](qreal anchor_x, qreal anchor_y, qreal _angle){
            //坐标有问题，多选图元旋转暂时不实现
            onMultiSelectedItemsRotate(anchor_x, anchor_y, _angle);
        });
    }
}

void GraphicsScene::destroyMultiSelector(){
    if(m_multiSelector != NULL){
        m_multiSelector->Remove(this);
        m_multiSelector->deleteLater();
        m_multiSelector = NULL;
    }
}

QSizeF GraphicsScene::GetViewSize() const{
    return  m_pView->size();
}

void GraphicsScene::OnSceneChanged(){
    if(m_stUndo.size() > 0){
        emit sigUndoAvailable(true);
    }else{
        emit sigUndoAvailable(false);
    }
    if(m_stRedo.size() > 0){
        emit sigRedoAvailable(true);
    }else{
        emit sigRedoAvailable(false);
    }
}

void GraphicsScene::Undo(){
    ACTION_NODE node = m_stUndo.pop();
    if(node.action == act_type_create_item){
        onItemRemove(node.shape->GetItemKey(), false);
    }else if(node.action == act_type_change_geo){
        node.shape->Undo();
        ModifyItemInProtobuf(node.shape);
        SaveProtobufToFile();
    }else if(node.action == act_type_remove_item){
        addItem(node.shape->GetGraphicsItem());
        ModifyItemInProtobuf(node.shape);
        SaveProtobufToFile();
    }else if(node.action == act_type_erase){
        UndoEraser(dynamic_cast<Eraser *>(node.shape));
    }
    MapShape::iterator it = m_mapShape.find(node.shape->GetItemKey());
    if(it != m_mapShape.end()){
        it.value()->isActivated = false;
    }
    m_stRedo.push(node);
    OnSceneChanged();
}

void GraphicsScene::Redo(){
    ACTION_NODE node = m_stRedo.pop();
    if(node.action == act_type_create_item){
        addItem(node.shape->GetGraphicsItem());
        ModifyItemInProtobuf(node.shape);
        SaveProtobufToFile();
    }else if(node.action == act_type_change_geo){
        node.shape->Redo();
        ModifyItemInProtobuf(node.shape);
        SaveProtobufToFile();
    }else if(node.action == act_type_remove_item){
        onItemRemove(node.shape->GetItemKey(), false);
    }else if(node.action == act_type_erase){
        RedoEraser(dynamic_cast<Eraser *>(node.shape));
    }
    MapShape::iterator it = m_mapShape.find(node.shape->GetItemKey());
    if(it != m_mapShape.end()){
        it.value()->isActivated = true;
    }
    m_stUndo.push(node);
    OnSceneChanged();
}

void GraphicsScene::UndoEraser(Eraser *eraser){
    for(MapShape::Iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
        it.value()->shape->UndoEraser(reinterpret_cast<int>(eraser));
    }
}

void GraphicsScene::RedoEraser(Eraser *eraser){
    for(MapShape::Iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
        it.value()->shape->RedoEraser(reinterpret_cast<int>(eraser));
    }
}

void GraphicsScene::SaveItemToProtobuf(ShapeBase *shape){
    TOOL_TYPE::Type type = shape->GetType();
    int itemKey = shape->GetItemKey();
    if(type ==TOOL_TYPE::LINE){
        QPointF pos = shape->GetPos();
        PBShape::Line *line = PBShapeHelper::CreateLine(pos.x(), pos.y(),
                                                        shape->GetP1().toPoint(), shape->GetP2().toPoint(),
                                                        shape->GetPen());
        (*m_pScenePb->mutable_mapline())[itemKey] = *line;
    }else if(type ==TOOL_TYPE::RECTANGLE){
        QPointF pos = shape->GetPos();
        int width = shape->GetItemWidth();
        int height = shape->GetItemHeight();
        PBShape::Rectangle *rectagle = PBShapeHelper::CreateRectangle(pos.x(), pos.y(), width, height,
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_maprectangle())[itemKey] = *rectagle;
    }else if(type ==TOOL_TYPE::ELLIPSE){
        QPointF pos = shape->GetPos();
        int width = shape->GetItemWidth();
        int height = shape->GetItemHeight();
        PBShape::Ellipse *ellipse = PBShapeHelper::CreateEllipse(pos.x(), pos.y(), width, height,
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_mapellipse())[itemKey] = *ellipse;
    }else if(type ==TOOL_TYPE::TRIANGLE){
        QPointF pos = shape->GetPos();
        int width = shape->GetItemWidth();
        int height = shape->GetItemHeight();
        PBShape::Triangle *triangle = PBShapeHelper::CreateTriangle(pos.x(), pos.y(), width, height,
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_maptriangle())[itemKey] = *triangle;
    }else if(type ==TOOL_TYPE::PENCIL){
        Pencil *pencil = dynamic_cast<Pencil *>(shape);
        QRect rc = shape->GetRect();
        QPen p = shape->GetPen();
        QPointF pos = shape->GetPos();
        QByteArray pathBa;
        pencil->GetPathData(&pathBa);
        QList<ERASER_DATA> *listEraser = pencil->GetEraserData();
        PBShape::Scribble *scribble = PBShapeHelper::CreateScribble(rc.x(), rc.y(), rc.width(), rc.height(), pos, p, pathBa, *listEraser);
        (*m_pScenePb->mutable_mapscribble())[itemKey] = *scribble;
        pencil->SafeDelete(listEraser);
    }
}

void GraphicsScene::ModifyItemInProtobuf(ShapeBase *shape){
    TOOL_TYPE::Type type = shape->GetType();
    int itemKey = shape->GetItemKey();
    if(type ==TOOL_TYPE::LINE){
        auto it = m_pScenePb->mutable_mapline()->find(itemKey);
        if(it != m_pScenePb->mutable_mapline()->end()){
            QPoint pos = shape->GetPos().toPoint();
            QPoint p0 = shape->GetP1().toPoint();
            QPoint p1 = shape->GetP2().toPoint();
            it->second.mutable_pos()->set_x(pos.x());
            it->second.mutable_pos()->set_y(pos.y());
            it->second.mutable_p0()->set_x(p0.x());
            it->second.mutable_p0()->set_y(p0.y());
            it->second.mutable_p1()->set_x(p1.x());
            it->second.mutable_p1()->set_y(p1.y());
        }
    }else if(type ==TOOL_TYPE::RECTANGLE){
        auto it = m_pScenePb->mutable_maprectangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maprectangle()->end()){
            QPointF p = shape->GetPos();
            int width = shape->GetItemWidth();
            int height = shape->GetItemHeight();
            qreal angle = shape->GetAngle();
            PBShape::Pos *pos = new PBShape::Pos();
            pos->set_x(p.x());
            pos->set_y(p.y());
            it->second.set_allocated_pos(pos);
            it->second.set_width(width);
            it->second.set_height(height);
            it->second.set_angle(angle);
        }
    }else if(type ==TOOL_TYPE::ELLIPSE){
        auto it = m_pScenePb->mutable_mapellipse()->find(itemKey);
        if(it != m_pScenePb->mutable_mapellipse()->end()){
            QPointF p = shape->GetPos();
            int width = shape->GetItemWidth();
            int height = shape->GetItemHeight();
            qreal angle = shape->GetAngle();
            PBShape::Pos *pos = new PBShape::Pos();
            pos->set_x(p.x());
            pos->set_y(p.y());
            it->second.set_allocated_pos(pos);
            it->second.set_width(width);
            it->second.set_height(height);
            it->second.set_angle(angle);
        }
    }else if(type ==TOOL_TYPE::TRIANGLE){
        auto it = m_pScenePb->mutable_maptriangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maptriangle()->end()){
            QPointF p = shape->GetPos();
            int width = shape->GetItemWidth();
            int height = shape->GetItemHeight();
            qreal angle = shape->GetAngle();
            PBShape::Pos *pos = new PBShape::Pos();
            pos->set_x(p.x());
            pos->set_y(p.y());
            it->second.set_allocated_pos(pos);
            it->second.set_width(width);
            it->second.set_height(height);
            it->second.set_angle(angle);
        }
    }else if(type ==TOOL_TYPE::PENCIL){
        auto it = m_pScenePb->mutable_mapscribble()->find(itemKey);
        if(it != m_pScenePb->mutable_mapscribble()->end()){
            QRect rc = shape->GetRect();
            it->second.mutable_rect()->set_x(rc.x());
            it->second.mutable_rect()->set_y(rc.y());
            it->second.mutable_rect()->set_w(rc.width());
            it->second.mutable_rect()->set_h(rc.height());

            QPointF pos = shape->GetPos();
            it->second.mutable_pos()->set_x(pos.x());
            it->second.mutable_pos()->set_y(pos.y());

            Pencil *pencil = dynamic_cast<Pencil *>(shape);
            QPen p = shape->GetPen();
            QByteArray pathBa;
            pencil->GetPathData(&pathBa);
            QList<ERASER_DATA> *listEraser = pencil->GetEraserData();

            PBShape::Scribble *scribble = &(it->second);
            it->second.set_path(pathBa.toStdString());
            it->second.mutable_maperaser()->clear();
            int i = 1;
            for(QList<ERASER_DATA>::const_iterator it = listEraser->constBegin(); it != listEraser->constEnd(); it++){
                ERASER_DATA edata = *it;
                PBShape::Eraser *eraser = new PBShape::Eraser();
                eraser->set_width(edata.width);
                QByteArray *eba = edata.ba;
                eraser->set_allocated_path(new std::string(eba->toStdString()));
                (*scribble->mutable_maperaser())[i++] = *eraser;
            }
            pencil->SafeDelete(listEraser);
        }
    }
}

void GraphicsScene::RemoveItemFromProtobuf(ShapeBase *shape){
    TOOL_TYPE::Type type = shape->GetType();
    int itemKey = shape->GetItemKey();
    if(type ==TOOL_TYPE::LINE){
        auto it = m_pScenePb->mutable_mapline()->find(itemKey);
        if(it != m_pScenePb->mutable_mapline()->end()){
            it->second.mutable_p0()->Clear();
            it->second.mutable_p1()->Clear();
            it->second.mutable_pen()->Clear();
            it->second.Clear();
            it = m_pScenePb->mutable_mapline()->erase(it);
        }
    }else if(type ==TOOL_TYPE::RECTANGLE){
        auto it = m_pScenePb->mutable_maprectangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maprectangle()->end()){
            it->second.mutable_pen()->Clear();
            it->second.mutable_pos()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_maprectangle()->erase(it);
        }
    }else if(type ==TOOL_TYPE::ELLIPSE){
        auto it = m_pScenePb->mutable_mapellipse()->find(itemKey);
        if(it != m_pScenePb->mutable_mapellipse()->end()){
            it->second.mutable_pen()->Clear();
            it->second.mutable_pos()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_mapellipse()->erase(it);
        }
    }else if(type ==TOOL_TYPE::TRIANGLE){
        auto it = m_pScenePb->mutable_maptriangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maptriangle()->end()){
            it->second.mutable_pen()->Clear();
            it->second.mutable_pos()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_maptriangle()->erase(it);
        }
    }else if(type ==TOOL_TYPE::PENCIL){
        auto it = m_pScenePb->mutable_mapscribble()->find(itemKey);
        if(it != m_pScenePb->mutable_mapscribble()->end()){
            it->second.mutable_rect()->Clear();
            it->second.mutable_path()->clear();
            it->second.mutable_pos()->Clear();
            it->second.mutable_maperaser()->clear();
            it = m_pScenePb->mutable_mapscribble()->erase(it);
        }
    }
}

//save protobuf to file
void GraphicsScene::SaveProtobufToFile(){
    int size = m_pScenePb->ByteSizeLong();
    char *pBuf = new char[size];
    int readSize = seriliazeData(*m_pScenePb, pBuf, size);
    emit sigWriteSceneObject(pBuf, readSize, m_strSceneObjectFileName);
}

void GraphicsScene::slotWriteSceneObjectCompleted(bool res, const char *data, const QString &fileName){
    delete []data;
}

//load items from protobuf file
void GraphicsScene::LoadSceneFromFile(const QString &fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件真正被其他进程使用，无法打开"));
        return;
    }
    QByteArray ba = file.readAll();
    file.close();
    PBShape::Scene scene;
    if(!scene.ParseFromArray(ba.data(), ba.size())){
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件已损坏，无法打开"));
        return;
    }

    for (auto it = scene.mapline().begin(); it != scene.mapline().end(); it ++) {
        PBShape::Line line = it->second;
        LoadLineObject(line);
    }
    for (auto it = scene.maprectangle().begin(); it != scene.maprectangle().end(); it ++) {
        PBShape::Rectangle rectangle = it->second;
        LoadRectangleObject(rectangle);
    }
    for (auto it = scene.mapellipse().begin(); it != scene.mapellipse().end(); it ++) {
        PBShape::Ellipse ellipse = it->second;
        LoadEllipseObject(ellipse);
    }
    for (auto it = scene.maptriangle().begin(); it != scene.maptriangle().end(); it ++) {
        PBShape::Triangle triangle = it->second;
        LoadTriangleObject(triangle);
    }
    for (auto it = scene.mapscribble().begin(); it != scene.mapscribble().end(); it ++) {
        PBShape::Scribble scribble = it->second;
        LoadScribbleObject(scribble);
    }
}

void GraphicsScene::LoadLineObject(const PBShape::Line &line){
    ShapeBase *shape = new Line(this);
    QPointF beginPoint = QPointF(line.p0().x(), line.p0().y());
    QPointF endPoint = QPointF(line.p1().x(), line.p1().y());
    m_nCurKey = shape->Create(beginPoint, endPoint, this);

    QColor penColor = QColor(line.pen().r(), line.pen().g(), line.pen().b(), line.pen().a());
    int penWidth = line.pen().width();
    Qt::PenStyle style = (Qt::PenStyle)line.pen().style();
    Qt::PenCapStyle capstyle = (Qt::PenCapStyle)line.pen().capstyle();
    Qt::PenJoinStyle joinstyle = (Qt::PenJoinStyle)line.pen().joinstyle();
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(style);
    pen.setCapStyle(capstyle);
    pen.setJoinStyle(joinstyle);
    shape->SetPen(pen);

    Line *lineItem = dynamic_cast<Line *>(shape);
    QPointF pos = QPointF(line.pos().x(), line.pos().y());
    lineItem->SetPos(pos);

    m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key, true);
    });
    connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
        onItemGeoChanged(_key);
    });

    (*m_pScenePb->mutable_mapline())[m_nCurKey] = line;

    shape->UpdateRect(beginPoint, endPoint, this);
    shape->CreateEnd(this);
}

void GraphicsScene::LoadRectangleObject(const PBShape::Rectangle &rectangle){
    ShapeBase *shape = new Rectangle(this);

    QPointF p1 = QPointF(rectangle.pos().x(), rectangle.pos().y());
    m_nCurKey = shape->Create(p1, p1+QPointF(1,1), this);

    QColor penColor = QColor(rectangle.pen().r(), rectangle.pen().g(), rectangle.pen().b(), rectangle.pen().a());
    int penWidth = rectangle.pen().width();
    Qt::PenStyle style = (Qt::PenStyle)rectangle.pen().style();
    Qt::PenCapStyle capstyle = (Qt::PenCapStyle)rectangle.pen().capstyle();
    Qt::PenJoinStyle joinstyle = (Qt::PenJoinStyle)rectangle.pen().joinstyle();
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(style);
    pen.setCapStyle(capstyle);
    pen.setJoinStyle(joinstyle);
    shape->SetPen(pen);
    QBrush brush;
    QColor brushColor = QColor(rectangle.brush().r(), rectangle.brush().g(), rectangle.brush().b(), rectangle.brush().a());
    brush.setColor(brushColor);
    Qt::BrushStyle brushStyle = (Qt::BrushStyle)rectangle.brush().style();
    brush.setStyle(brushStyle);
    shape->SetBrush(brush);

    m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key, true);
    });
    connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
        onItemGeoChanged(_key);
    });
    (*m_pScenePb->mutable_maprectangle())[m_nCurKey] = rectangle;

    qreal rWidth = rectangle.width();
    qreal rHeight = rectangle.height();
    QPointF beginPoint = QPointF(p1.x()-rWidth/2, p1.y()-rHeight/2);
    QPointF endPoint = QPointF(p1.x()+rWidth/2, p1.y()+rHeight/2);
    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = rectangle.angle();
    shape->Rotate(0, 0, angle);
    shape->CreateEnd(this);
}

void GraphicsScene::LoadEllipseObject(const PBShape::Ellipse &ellipse){
    ShapeBase *shape = new Ellipse(this);
    QPointF p1 = QPointF(ellipse.pos().x(), ellipse.pos().y());
    m_nCurKey = shape->Create(p1, p1+QPointF(1,1), this);

    QColor penColor = QColor(ellipse.pen().r(), ellipse.pen().g(), ellipse.pen().b(), ellipse.pen().a());
    int penWidth = ellipse.pen().width();
    Qt::PenStyle style = (Qt::PenStyle)ellipse.pen().style();
    Qt::PenCapStyle capstyle = (Qt::PenCapStyle)ellipse.pen().capstyle();
    Qt::PenJoinStyle joinstyle = (Qt::PenJoinStyle)ellipse.pen().joinstyle();
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(style);
    pen.setCapStyle(capstyle);
    pen.setJoinStyle(joinstyle);
    shape->SetPen(pen);
    QBrush brush;
    QColor brushColor = QColor(ellipse.brush().r(), ellipse.brush().g(), ellipse.brush().b(), ellipse.brush().a());
    brush.setColor(brushColor);
    Qt::BrushStyle brushStyle = (Qt::BrushStyle)ellipse.brush().style();
    brush.setStyle(brushStyle);
    shape->SetBrush(brush);

    m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key, true);
    });
    connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
        onItemGeoChanged(_key);
    });

    (*m_pScenePb->mutable_mapellipse())[m_nCurKey] = ellipse;

    qreal rWidth = ellipse.width();
    qreal rHeight = ellipse.height();
    QPointF beginPoint = QPointF(p1.x()-rWidth/2, p1.y()-rHeight/2);
    QPointF endPoint = QPointF(p1.x()+rWidth/2, p1.y()+rHeight/2);
    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = ellipse.angle();
    shape->Rotate(0, 0, angle);

    shape->CreateEnd(this);
}

void GraphicsScene::LoadTriangleObject(const PBShape::Triangle &triangle){
    ShapeBase *shape = new Triangle(this);
    QPointF p1 = QPointF(triangle.pos().x(), triangle.pos().y());
    m_nCurKey = shape->Create(p1, p1+QPointF(1,1), this);

    QColor penColor = QColor(triangle.pen().r(), triangle.pen().g(), triangle.pen().b(), triangle.pen().a());
    int penWidth = triangle.pen().width();
    Qt::PenStyle style = (Qt::PenStyle)triangle.pen().style();
    Qt::PenCapStyle capstyle = (Qt::PenCapStyle)triangle.pen().capstyle();
    Qt::PenJoinStyle joinstyle = (Qt::PenJoinStyle)triangle.pen().joinstyle();
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(style);
    pen.setCapStyle(capstyle);
    pen.setJoinStyle(joinstyle);
    shape->SetPen(pen);
    QBrush brush;
    QColor brushColor = QColor(triangle.brush().r(), triangle.brush().g(), triangle.brush().b(), triangle.brush().a());
    brush.setColor(brushColor);
    Qt::BrushStyle brushStyle = (Qt::BrushStyle)triangle.brush().style();
    brush.setStyle(brushStyle);
    shape->SetBrush(brush);

    m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key, true);
    });
    connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
        onItemGeoChanged(_key);
    });
    (*m_pScenePb->mutable_maptriangle())[m_nCurKey] = triangle;

    qreal rWidth = triangle.width();
    qreal rHeight = triangle.height();
    QPointF beginPoint = QPointF(p1.x()-rWidth/2, p1.y()-rHeight/2);
    QPointF endPoint = QPointF(p1.x()+rWidth/2, p1.y()+rHeight/2);
    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = triangle.angle();
    shape->Rotate(0, 0, angle);
    shape->CreateEnd(this);
}

void GraphicsScene::LoadScribbleObject(const PBShape::Scribble &scribble){
    ShapeBase *shape = new Pencil(this);
    Pencil *pencil = dynamic_cast<Pencil *>(shape);
    QRect rect = QRect(scribble.rect().x(), scribble.rect().y(), scribble.rect().w(),scribble.rect().h());
    m_nCurKey = shape->Create(rect.topLeft(), rect.bottomRight(), this);

    QColor penColor = QColor(scribble.pen().r(), scribble.pen().g(), scribble.pen().b(), scribble.pen().a());
    int penWidth = scribble.pen().width();
    Qt::PenStyle style = (Qt::PenStyle)scribble.pen().style();
    Qt::PenCapStyle capstyle = (Qt::PenCapStyle)scribble.pen().capstyle();
    Qt::PenJoinStyle joinstyle = (Qt::PenJoinStyle)scribble.pen().joinstyle();
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(penWidth);
    pen.setStyle(style);
    pen.setCapStyle(capstyle);
    pen.setJoinStyle(joinstyle);
    shape->SetPen(pen);

    QByteArray pathBa = QByteArray::fromStdString(scribble.path());
    QDataStream in(pathBa);
    QPainterPath path;
    in >> path;
    pencil->SetPath(path);

    QPointF pos = QPointF(scribble.pos().x(), scribble.pos().y());
    pencil->SetPos(pos);

    for(auto it = scribble.maperaser().begin(); it != scribble.maperaser().end(); it++){
        QByteArray eraserBa = QByteArray::fromStdString(it->second.path());
        int eraserWidth = it->second.width();
        QDataStream in_e(eraserBa);
        QPainterPath path_e;
        in_e >> path_e;
        pencil->AddEraserPath(path_e, eraserWidth);
    }

    m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key, true);
    });

    connect(shape, &ShapeBase::sigGeoChanged, [=](int _key){
        onItemGeoChanged(_key);
    });

    connect(this, SIGNAL(sigEraserMove(const QPointF &, const QPointF &, Eraser *)),
            pencil, SLOT(slotEraserMove(const QPointF &, const QPointF &, Eraser *)));
    connect(this, SIGNAL(sigEraserPressed()),
            pencil, SLOT(slotEraserPressed()));
    connect(this, SIGNAL(sigEraserRelease()),
            pencil, SLOT(slotEraserRelease()));
    connect(pencil, SIGNAL(sigEraserAttach(int)),
            this, SLOT(slotEraserAttachToPencil(int)));

    (*m_pScenePb->mutable_mapscribble())[m_nCurKey] = scribble;

    shape->CreateEnd(this);
}

void GraphicsScene::slotEraserAttachToPencil(int key){
    qDebug()<<"GraphicsScene::onItemGeoChanged key="<<key;
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        if(shape->GetType() == TOOL_TYPE::PENCIL){
            ModifyItemInProtobuf(shape);
            SaveProtobufToFile();
            OnSceneChanged();
        }
    }
}

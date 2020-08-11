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

const qreal PI = 3.1415926;
const qreal ARC = PI/180;

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

    m_stRedo.clear();
    m_stUndo.clear();

    clearScene();
    deleteSelectItem();
    destroyMultiSelector();
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
            onItemRemove(_key);
        });
        //创建新元素，清空所有redo
        m_stRedo.clear();
        m_stUndo.push(ACTION_NODE(act_type_create_item, shape));
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
    shape->CreateEnd(event->scenePos(), this);
    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            break;
        }
        case TOOL_TYPE::PENCIL:
        case TOOL_TYPE::RECTANGLE:
        case TOOL_TYPE::ELLIPSE:
        case TOOL_TYPE::TRIANGLE:
        case TOOL_TYPE::LINE:{
            SaveItemToProtobuf(shape);
            SaveProtobufToFile();
            break;
        }
        case TOOL_TYPE::ERASER:{
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

void GraphicsScene::SelectItemsByRubberBand(const QRectF &rubberBandRect){
//    m_listSelectedItems.clear();
    resetMultiSelector();
    destroyMultiSelector();

    for(MapShape::iterator it = m_mapShape.begin(); it != m_mapShape.end(); it++){
        QRectF rcShape = it.value()->shape->GetRect();
        if(rcShape.intersects(rubberBandRect)){
            it.value()->shape->SetSelected(true);
            it.value()->shape->HideControls(true);
            m_listSelectedItems.push_back(it.value()->key);
        }
    }
    if(m_listSelectedItems.size() == 1){
        m_mapShape.find(m_listSelectedItems.first()).value()->shape->HideControls(false);
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
        //it.value()->shape->SetEditable(true);
        qDebug() << "content item selected";
    }else{
        qDebug() << "selector item selected, or no items selected";
        //destroyMultiSelector();
    }
    //signalItemSelected(key);
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

void GraphicsScene::clearScene(){
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
    m_pView->update();
}

void GraphicsScene::onItemPosChanged(int key, qreal dx, qreal dy){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        shape->ChangePos(dx, dy);
    }
}

void GraphicsScene::onItemRemove(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        ShapeBase *shape = data->shape;
        RemoveItemFromProtobuf(shape);
        removeItem(shape->GetGraphicsItem());
        delete shape;
        delete data;
        it = m_mapShape.erase(it);
        SaveProtobufToFile();
    }
}

void GraphicsScene::onItemsRemoveByRubberBand(){
    for(ListShapeKey::iterator it = m_listSelectedItems.begin(); it != m_listSelectedItems.end(); ){
        onItemRemove(*it);
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

void GraphicsScene::onItemResizeBegin(int key){

}

void GraphicsScene::onItemResize(int key, qreal dx, qreal dy){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;
    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    shape->ChangeSize(dx, dy);

    QRectF rcSceneShape = shape->GetRect();
    QPoint topLeftView = m_pView->mapFromScene(rcSceneShape.topLeft());
    QPoint bottomRightView = m_pView->mapFromScene(rcSceneShape.bottomRight());
    QRect rcViewShape = QRect(topLeftView, bottomRightView);
    QPointF p1 = m_pView->mapFromScene(it.value()->shape->GetP1());
    QPointF p2 = m_pView->mapFromScene(it.value()->shape->GetP2());
    emit sigItemPointsChanged(key, data->toolType, rcViewShape, p1, p2);

}

void GraphicsScene::onItemResizeEnd(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;

    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    QRectF rcSceneShape = shape->GetRect();
    QPoint topLeftView = m_pView->mapFromScene(rcSceneShape.topLeft());
    QPoint bottomRightView = m_pView->mapFromScene(rcSceneShape.bottomRight());
    QRect rcViewShape = QRect(topLeftView, bottomRightView);
    QPointF p1 = m_pView->mapFromScene(it.value()->shape->GetP1());
    QPointF p2 = m_pView->mapFromScene(it.value()->shape->GetP2());

    emit sigItemResizeCompleted(key, data->toolType, rcViewShape, p1, p2);

    signalItemSelected(key);

    m_pView->update();
}

void GraphicsScene::onItemRotateBegin(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;

    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    shape->RotateBegin();
}

void GraphicsScene::onItemRotate(int key, qreal angle){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;

    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    shape->Rotate(0, 0, angle);
}

void GraphicsScene::onItemRotateEnd(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;

    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    shape->RotateEnd();

    signalItemSelected(key);

    m_pView->update();
}

void GraphicsScene::signalItemSelected(int key){
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        it.value()->shape->SetSelected(true);
        QRect rcSceneShape = it.value()->shape->GetRect();
        QPoint topLeftView = m_pView->mapFromScene(rcSceneShape.topLeft());
        QPoint bottomRightView = m_pView->mapFromScene(rcSceneShape.bottomRight());
        QRect rcViewShape = QRect(topLeftView, bottomRightView);
        QPointF lineP1 = it.value()->shape->GetP1();
        QPointF lineP2 = it.value()->shape->GetP2();
        QPointF p1 = m_pView->mapFromScene(lineP1);
        QPointF p2 = m_pView->mapFromScene(lineP2);

        qDebug()<<"GraphicsScene::signalItemSelected lineP1="<<lineP1<<", lineP2="<<lineP2<<"; p1="<<p1<<", p2"<<p2;
//        QPointF p0 = (p1+p2)/2;
//        qreal angle = it.value()->shape->GetAngle();
//        QPointF p1_a = QPointF((p1.x()-p0.x())*cos(angle*ARC)-(p1.y()-p0.y())*sin(angle*ARC) + p0.x(),
//                               (p1.x()-p0.x())*sin(angle*ARC)+(p1.y()-p0.y())*cos(angle*ARC) + p0.y());
//        QPointF p2_a = QPointF((p2.x()-p0.x())*cos(angle*ARC)-(p2.y()-p0.y())*sin(angle*ARC) + p0.x(),
//                               (p2.x()-p0.x())*sin(angle*ARC)+(p2.y()-p0.y())*cos(angle*ARC) + p0.y());

        emit sigItemSelected(key, it.value()->toolType, rcViewShape, p1, p2);
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
        onItemRemove(node.shape->GetItemKey());
    }
    m_stRedo.push(node);
    OnSceneChanged();
}

void GraphicsScene::Redo(){
    ACTION_NODE node = m_stRedo.pop();
    if(node.action == act_type_create_item){

    }
    m_stUndo.push(node);
    OnSceneChanged();
}

void GraphicsScene::SaveItemToProtobuf(ShapeBase *shape){
    TOOL_TYPE::Type type = shape->GetType();
    int itemKey = shape->GetItemKey();
    if(type ==TOOL_TYPE::LINE){
        PBShape::Line *line = PBShapeHelper::CreateLine(shape->GetP1().toPoint(), shape->GetP2().toPoint(), shape->GetPen());
        (*m_pScenePb->mutable_mapline())[itemKey] = *line;
    }else if(type ==TOOL_TYPE::RECTANGLE){
        QRect rc = shape->GetRect();
        PBShape::Rectangle *rectagle = PBShapeHelper::CreateRectangle(rc.x(), rc.y(), rc.width(), rc.height(),
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_maprectangle())[itemKey] = *rectagle;
    }else if(type ==TOOL_TYPE::ELLIPSE){
        QRect rc = shape->GetRect();
        PBShape::Ellipse *ellipse = PBShapeHelper::CreateEllipse(rc.x(), rc.y(), rc.width(), rc.height(),
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_mapellipse())[itemKey] = *ellipse;
    }else if(type ==TOOL_TYPE::TRIANGLE){
        QRect rc = shape->GetRect();
        PBShape::Triangle *triangle = PBShapeHelper::CreateTriangle(rc.x(), rc.y(), rc.width(), rc.height(),
                                                                      shape->GetPen(), shape->GetBrush());
        (*m_pScenePb->mutable_maptriangle())[itemKey] = *triangle;
    }else if(type ==TOOL_TYPE::PENCIL){
        QRect rc = shape->GetRect();
        QPixmap pixmap = shape->GetPixmap();
        QByteArray byteArray = QByteArray();
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer,"png",0);
//        QDataStream out;
//        out<<shape->GetRect();
        PBShape::Scribble *scribble = PBShapeHelper::CreateScribble(rc.x(), rc.y(), rc.width(), rc.height(), byteArray);
        (*m_pScenePb->mutable_mapscribble())[itemKey] = *scribble;
    }
}

void GraphicsScene::ModifyItemInProtobuf(ShapeBase *shape){
    TOOL_TYPE::Type type = shape->GetType();
    int itemKey = shape->GetItemKey();
    if(type ==TOOL_TYPE::LINE){
        auto it = m_pScenePb->mutable_mapline()->find(itemKey);
        if(it != m_pScenePb->mutable_mapline()->end()){
            QPoint p0 = shape->GetP1().toPoint();
            QPoint p1 = shape->GetP2().toPoint();
            it->second.mutable_p0()->set_x(p0.x());
            it->second.mutable_p0()->set_y(p0.y());
            it->second.mutable_p1()->set_x(p1.x());
            it->second.mutable_p1()->set_y(p1.y());
        }
    }else if(type ==TOOL_TYPE::RECTANGLE){
        auto it = m_pScenePb->mutable_maprectangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maprectangle()->end()){
            QRect rc = shape->GetRect();
            it->second.mutable_rect()->set_x(rc.x());
            it->second.mutable_rect()->set_y(rc.y());
            it->second.mutable_rect()->set_w(rc.width());
            it->second.mutable_rect()->set_h(rc.height());
        }
    }else if(type ==TOOL_TYPE::ELLIPSE){
        auto it = m_pScenePb->mutable_mapellipse()->find(itemKey);
        if(it != m_pScenePb->mutable_mapellipse()->end()){
            QRect rc = shape->GetRect();
            it->second.mutable_rect()->set_x(rc.x());
            it->second.mutable_rect()->set_y(rc.y());
            it->second.mutable_rect()->set_w(rc.width());
            it->second.mutable_rect()->set_h(rc.height());
        }
    }else if(type ==TOOL_TYPE::TRIANGLE){
        auto it = m_pScenePb->mutable_maptriangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maptriangle()->end()){
            QRect rc = shape->GetRect();
            it->second.mutable_rect()->set_x(rc.x());
            it->second.mutable_rect()->set_y(rc.y());
            it->second.mutable_rect()->set_w(rc.width());
            it->second.mutable_rect()->set_h(rc.height());
        }
    }else if(type ==TOOL_TYPE::PENCIL){
        auto it = m_pScenePb->mutable_mapscribble()->find(itemKey);
        if(it != m_pScenePb->mutable_mapscribble()->end()){
            QRect rc = shape->GetRect();
            it->second.mutable_rect()->set_x(rc.x());
            it->second.mutable_rect()->set_y(rc.y());
            it->second.mutable_rect()->set_w(rc.width());
            it->second.mutable_rect()->set_h(rc.height());

            QPixmap pixmap = shape->GetPixmap();
            QByteArray ba = QByteArray();
            QBuffer buff(&ba);
            pixmap.save(&buff, "png", 100);
            it->second.set_pixmap(ba.data(), ba.size());
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
            it->second.mutable_rect()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_maprectangle()->erase(it);
        }
    }else if(type ==TOOL_TYPE::ELLIPSE){
        auto it = m_pScenePb->mutable_mapellipse()->find(itemKey);
        if(it != m_pScenePb->mutable_mapellipse()->end()){
            it->second.mutable_pen()->Clear();
            it->second.mutable_rect()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_mapellipse()->erase(it);
        }
    }else if(type ==TOOL_TYPE::TRIANGLE){
        auto it = m_pScenePb->mutable_maptriangle()->find(itemKey);
        if(it != m_pScenePb->mutable_maptriangle()->end()){
            it->second.mutable_pen()->Clear();
            it->second.mutable_rect()->Clear();
            it->second.mutable_brush()->Clear();
            it->second.Clear();
            it  = m_pScenePb->mutable_maptriangle()->erase(it);
        }
    }else if(type ==TOOL_TYPE::PENCIL){
        auto it = m_pScenePb->mutable_mapscribble()->find(itemKey);
        if(it != m_pScenePb->mutable_mapscribble()->end()){
            it->second.mutable_rect()->Clear();
            it->second.mutable_pixmap()->clear();
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

    QPointF beginPoint = QPointF(line.p0().x(), line.p0().y());
    QPointF endPoint = QPointF(line.p1().x(), line.p1().y());

    int itemKey = shape->Create(beginPoint, endPoint, this);
    m_mapShape[itemKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key);
    });
    (*m_pScenePb->mutable_mapline())[itemKey] = line;

    shape->UpdateRect(beginPoint, endPoint, this);
}

void GraphicsScene::LoadRectangleObject(const PBShape::Rectangle &rectangle){
    ShapeBase *shape = new Rectangle(this);
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

    QPointF beginPoint = QPointF(rectangle.rect().x(), rectangle.rect().y());
    QPointF endPoint = QPointF(rectangle.rect().x() + rectangle.rect().w(), rectangle.rect().y() + rectangle.rect().h());

    int itemKey = shape->Create(beginPoint, endPoint, this);
    m_mapShape[itemKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key);
    });
    (*m_pScenePb->mutable_maprectangle())[itemKey] = rectangle;

    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = rectangle.angle();
    shape->RotateBegin();
    //shape->Rotate(angle, this);
}

void GraphicsScene::LoadEllipseObject(const PBShape::Ellipse &ellipse){
    ShapeBase *shape = new Ellipse(this);
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

    QPointF beginPoint = QPointF(ellipse.rect().x(), ellipse.rect().y());
    QPointF endPoint = QPointF(ellipse.rect().x() + ellipse.rect().w(), ellipse.rect().y() + ellipse.rect().h());

    int itemKey = shape->Create(beginPoint, endPoint, this);
    m_mapShape[itemKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key);
    });

    (*m_pScenePb->mutable_mapellipse())[itemKey] = ellipse;

    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = ellipse.angle();
    shape->RotateBegin();
    //shape->Rotate(angle, this);
}

void GraphicsScene::LoadTriangleObject(const PBShape::Triangle &triangle){
    ShapeBase *shape = new Triangle(this);
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

    QPointF beginPoint = QPointF(triangle.rect().x(), triangle.rect().y());
    QPointF endPoint = QPointF(triangle.rect().x() + triangle.rect().w(), triangle.rect().y() + triangle.rect().h());

    int itemKey = shape->Create(beginPoint, endPoint, this);
    m_mapShape[itemKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key);
    });

    (*m_pScenePb->mutable_maptriangle())[itemKey] = triangle;

    shape->UpdateRect(beginPoint, endPoint, this);

    qreal angle = triangle.angle();
    shape->RotateBegin();
    //shape->Rotate(angle, this);
}

void GraphicsScene::LoadScribbleObject(const PBShape::Scribble &scribble){
    ShapeBase *shape = new Pencil(this);
    std::string pixData = scribble.pixmap();
    QPixmap pixmap;
    //pixmap.loadFromData(pixData.c_str(), pixData.size(), "png");
    QByteArray ba(pixData.data(), pixData.size());
    pixmap.loadFromData(ba, "png");
    QRect rect = QRect(scribble.rect().x(), scribble.rect().y(), scribble.rect().w(),scribble.rect().h());
    int itemKey = 0;//shape->LoadFromPixmap(pixmap, rect, this);
    m_mapShape[itemKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    connect(shape, &ShapeBase::sigRemove, [=](int _key){
        onItemRemove(_key);
    });

    (*m_pScenePb->mutable_mapscribble())[itemKey] = scribble;
}

#include "GraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include "src/shape/Rectangle.h"
#include "src/shape/Ellipse.h"
#include "src/shape/Triangle.h"
#include "src/shape/Line.h"
#include "src/shape/Pencil.h"
#include "src/shape/MultiSelector.h"
#include "src/graphics/GraphicsView.h"
#include <QDebug>

const qreal PI = 3.1415926;
const qreal ARC = PI/180;

inline QPixmap GetCursorPixmap(const QString &fileName){
    QPixmap pixmap(fileName);
    return pixmap;
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
{
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

    //SetAllEditable(false);
    deleteSelectItem();
//    UnselectedAll();
//    destroyMultiSelector();

    m_bPressed = true;
    QPointF pos = event->scenePos();
    m_ptPrev = pos;

    //QPoint viewPos = m_pView->mapFromScene(pos);
    //qDebug() << "GraphicsScene::mousePressEvent scene pos = " << pos << ", view pos = "<<viewPos;

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
            break;
        }
        case TOOL_TYPE::UNDO:{
            break;
        }
        case TOOL_TYPE::REDO:{
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
        case TOOL_TYPE::RECTANGLE:
        case TOOL_TYPE::ELLIPSE:
        case TOOL_TYPE::LINE:
        case TOOL_TYPE::TRIANGLE:{
            shape->UpdateRect(m_ptPrev, pos, this);
            break;
        }
        case TOOL_TYPE::ERASER:{
            break;
        }
        case TOOL_TYPE::UNDO:{
            break;
        }
        case TOOL_TYPE::REDO:{
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
            break;
        }
        case TOOL_TYPE::PENCIL:{
            break;
        }
        case TOOL_TYPE::RECTANGLE:{
            break;
        }
        case TOOL_TYPE::ELLIPSE:{
            break;
        }
        case TOOL_TYPE::TRIANGLE:{
        }
        case TOOL_TYPE::LINE:{
            break;
        }
        case TOOL_TYPE::ERASER:{
            break;
        }
        case TOOL_TYPE::UNDO:{
            break;
        }
        case TOOL_TYPE::REDO:{
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
    MapShape::iterator it = m_mapShape.begin();
    while(it != m_mapShape.end()){
        SHAPE_DATA *data = it.value();
        data->shape->Remove(this);
        data->shape->deleteLater();
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
        removeItem(shape->GetGraphicsItem());
        shape->deleteLater();
        delete data;
        it = m_mapShape.erase(it);
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

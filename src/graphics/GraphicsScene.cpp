#include "GraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include "src/shape/Rectangle.h"
#include "src/shape/Ellipse.h"
#include "src/shape/Line.h"
#include "src/graphics/GraphicsView.h"
#include <QDebug>

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

    QGraphicsScene::mousePressEvent(event);
    m_bPressed = true;
    QPointF pos = event->scenePos();
    m_ptPrev = pos;

    QPoint viewPos = m_pView->mapFromScene(pos);
    qDebug() << "GraphicsScene::mousePressEvent scene pos = " << pos << ", view pos = "<<viewPos;

    ShapeBase *shape = nullptr;
    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            onMouseSelectItem(pos);
            break;
        }
        case TOOL_TYPE::PENCIL:{
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
        m_nCurKey = shape->Create(m_ptPrev, pos, this);
        m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, m_eToolType, shape, m_mapShape.size()+1);
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if(!m_bPressed){
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

    QPointF pos = event->scenePos();
    switch(m_eToolType){
        case TOOL_TYPE::SELECT:{
            break;
        }
        case TOOL_TYPE::PENCIL:{
            break;
        }
        case TOOL_TYPE::RECTANGLE:
        case TOOL_TYPE::ELLIPSE:
        case TOOL_TYPE::LINE:{
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
    m_bPressed = false;
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

void GraphicsScene::onMouseSelectItem(const QPointF &pos)
{
    QPointF itemPos = QPointF(pos.x(), pos.y());
    QGraphicsItem *item = this->itemAt(itemPos, QTransform());
    if(item == nullptr){
        return;
    }
    int key = item->data(ITEM_DATA_KEY).toInt();
    MapShape::iterator it = m_mapShape.find(key);
    if(it != m_mapShape.end()){
        it.value()->shape->SetSelected();
        QRect rcSceneShape = it.value()->shape->GetRect();
        QPoint topLeftView = m_pView->mapFromScene(rcSceneShape.topLeft());
        QPoint bottomRightView = m_pView->mapFromScene(rcSceneShape.bottomRight());
        QRect rcViewShape = QRect(topLeftView, bottomRightView);
        QPointF p1 = m_pView->mapFromScene(it.value()->shape->GetP1());
        QPointF p2 = m_pView->mapFromScene(it.value()->shape->GetP2());
        emit sigItemSelected(key, it.value()->toolType, rcViewShape, p1, p2);
    }
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

void GraphicsScene::onItemResizeBegin(int key){

}

void GraphicsScene::onItemResize(int key, qreal dx, qreal dy){
    MapShape::iterator it = m_mapShape.find(key);
    if(it == m_mapShape.end())
        return;
    SHAPE_DATA *data = it.value();
    ShapeBase *shape = data->shape;
    shape->ChangeSize(dx, dy);
    //if(data->toolType == TOOL_TYPE::LINE){
        QRectF rcSceneShape = shape->GetRect();
        QPoint topLeftView = m_pView->mapFromScene(rcSceneShape.topLeft());
        QPoint bottomRightView = m_pView->mapFromScene(rcSceneShape.bottomRight());
        QRect rcViewShape = QRect(topLeftView, bottomRightView);
        QPointF p1 = m_pView->mapFromScene(it.value()->shape->GetP1());
        QPointF p2 = m_pView->mapFromScene(it.value()->shape->GetP2());
        emit sigItemPointsChanged(key, data->toolType, rcViewShape, p1, p2);
    //}
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
}

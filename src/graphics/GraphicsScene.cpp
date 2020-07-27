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
    QGraphicsScene::mousePressEvent(event);
    m_bPressed = true;
    QPointF pos = event->scenePos();
    m_ptPrev = pos;

    qDebug() << "GraphicsScene::mousePressEvent pos = " << pos;

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
        m_mapShape[m_nCurKey] = new SHAPE_DATA(m_nCurKey, shape, m_mapShape.size()+1);
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
        emit sigItemSelected(key, it.value()->shape->GetRect());
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

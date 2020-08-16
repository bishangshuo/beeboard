#include "ItemBase.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <qmath.h>
#include <QDebug>
#include <QRandomGenerator>

const int MAX_ITEM_SIZE    = 800;
const int MIN_ITEM_SIZE    = 80;

//============================================================================//
//                                  ItemBase                                  //
//============================================================================//

ItemBase::ItemBase(int x, int y, int width, int height)
    : m_width(width)
    , m_height(height)
    , m_isResizing(false)
    , m_isRotating(false)
    , m_isCreating(true)
    , m_hideRotate(false)
    , m_hideClose(false)
    , m_hideResize(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    setPos(x, y);
}

ItemBase::~ItemBase()
{
}

QRectF ItemBase::boundingRect() const
{
    return QRectF(-m_width / 2, -m_height / 2, m_width, m_height);
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QColor color(255, 0, 0, 128);

    if (option->state & QStyle::State_Selected)
    {
        //绘制选中边框
        //if (option->state & QStyle::State_HasFocus)
        {
            QPen pen_r = painter->pen();
            int itemWidth  = pen_r.width();

            pen_r.setColor(color);
            pen_r.setWidth(1);
            pen_r.setStyle(Qt::DashLine);
            painter->setPen(pen_r);
            //选中边框
            painter->drawRect(-m_width/2-itemWidth/2-1, -m_height/2-itemWidth/2-1,  m_width+itemWidth+2, m_height+itemWidth+2);
        }

        if(!m_hideResize){
            //缩放按钮区域
            QPixmap px_m(":/resources/images/move.png");
            px_m = px_m.scaled(20, 20);
            painter->drawPixmap(m_width/2-20, m_height/2-20, px_m);
        }

        if(!m_hideRotate){
            //旋转按钮区域
            QPixmap px_r(":/resources/images/rotate-left.png");
            px_r = px_r.scaled(20, 20);
            painter->drawPixmap(-10, -m_height/2, px_r);
        }

        if(!m_hideClose){
            //关闭按钮区域
            QPixmap px_c(":/resources/images/close.png");
            px_c = px_c.scaled(20, 20);
            painter->drawPixmap(m_width/2-20, -m_height/2, px_c);
        }

    }

    painter->setRenderHint(QPainter::Antialiasing, false);
}

void ItemBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ((isInResizeArea(event->pos()) && isSelected()) && !m_hideResize){
        setCursor(Qt::SizeAllCursor);
    }else if ((isInRotateArea(event->pos()) && isSelected()) && !m_hideRotate)
    {
        QCursor *rotateCursor = new QCursor(QPixmap(":/resources/images/rotate-left-cur.png"));
        setCursor(*rotateCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void ItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_ptClicked = event->scenePos();
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInResizeArea(event->pos())  && !m_hideResize)
    {
        qDebug()<<"before resize, scenePos="<<scenePos();
        m_isResizing = true;
    }
    else if (event->button() == Qt::LeftButton && isInRotateArea(event->pos())  && !m_hideRotate)
    {
        m_isRotating = true;
    }
    else if (event->button() == Qt::LeftButton && isInCloseArea(event->pos())  && !m_hideClose)
    {
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }else{
        QGraphicsItem::mousePressEvent(event);
    }
}

void ItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isResizing && !m_hideResize)
    {
        int dx = int(2.0 * event->pos().x());
        int dy = int(2.0 * event->pos().y());
        prepareGeometryChange();

        m_width = dx;
        m_height = dy;
        if (m_width < MIN_ITEM_SIZE){
            m_width = MIN_ITEM_SIZE;
        }

        if (m_height < MIN_ITEM_SIZE){
            m_height = MIN_ITEM_SIZE;
        }

    }
    else if (m_isRotating && !m_hideRotate)
    {
        QPointF cursorPos = event->pos();
        QVector2D vectorStart = QVector2D(QPointF(0.0, -m_width / 2) - QPointF(0.0, 0.0));           // 起始向量
        QVector2D vectorEnd = QVector2D(cursorPos - QPointF(0.0, 0.0));                             // 结束向量
        // 计算起始向量和结束向量之间的角度
        qreal angle = 0.0;
        qreal angleEnd = GetDegreeAngle(vectorEnd);
        qreal angleStart = GetDegreeAngle(vectorStart);
        angle = angleEnd - angleStart + rotation();
        Rotate(angle);

        if(m_pCBRotate){
            QPointF _scenePos = scenePos();
            m_pCBRotate(_scenePos.x(), _scenePos.y(), angle);
        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isResizing)
    {
        m_isResizing = false;
    }
    else if (event->button() == Qt::LeftButton && m_isRotating)
    {
        m_isRotating = false;
    }
    else
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }

    QPointF pos = scenePos();
    BASEITEM_GEO *geo = new BASEITEM_GEO(pos.x(), pos.y(), m_width, m_height, rotation());
    BASEITEM_GEO *topGeo = m_stUndo.top();
    if(!topGeo->compare(*geo)){
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), m_width, m_height, rotation()));
    }

    if(m_ptClicked != event->scenePos())
        emit sigItemChanged();
}

int ItemBase::type() const
{
    return Type;
}


bool ItemBase::isInResizeArea(const QPointF &pos) const
{
    return QRectF(m_width/2-20, m_height/2-20, 20, 20).contains(pos);
}

bool ItemBase::isInRotateArea(const QPointF &pos) const
{
    return QRectF(-10, -m_height/2, 20, 20).contains(pos);
}

bool ItemBase::isInCloseArea(const QPointF &pos) const
{
    return QRectF(m_width/2-20, -m_height/2, 20, 20).contains(pos);
}


void ItemBase::Rotate(qreal angle){
    if (angle > 360.0)
    {
        while(1)
        {
            angle -= 360.0;
            if (angle < 360.0) break;
        }
    }
    else if (angle < 0.0)
    {
        while(1)
        {
            angle += 360.0;
            if (angle > 0.0) break;
        }
    }
    setRotation(angle);
}

void ItemBase::Created(){
    if(m_isCreating){
        QPointF pos = scenePos();
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), m_width, m_height, rotation()));
        m_isCreating = false;
    }

}

void ItemBase::Undo(){
    if(m_stUndo.size() < 2){
        return;
    }
    BASEITEM_GEO *geo_r = m_stUndo.pop();

    BASEITEM_GEO *geo = m_stUndo.top();
    QPointF pos = QPointF(geo->px, geo->py);
    qreal angle = geo->angle;
    m_width = geo->width;
    m_height = geo->height;
    setPos(pos);
    Rotate(angle);
    update();

    m_stRedo.push(geo_r);
}

void ItemBase::Redo(){
    if(m_stRedo.size() == 0){
        return;
    }
    BASEITEM_GEO *geo = m_stRedo.pop();
    QPointF pos = QPointF(geo->px, geo->py);
    qreal angle = geo->angle;
    m_width = geo->width;
    m_height = geo->height;
    setPos(pos);
    Rotate(angle);
    update();
    m_stUndo.push(geo);
}

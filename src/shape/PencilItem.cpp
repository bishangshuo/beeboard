#include "PencilItem.h"

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

PencilItem::PencilItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , m_isResizing(false)
    , m_isRotating(false)
    , m_isCreating(true)
    , m_hideRotate(false)
    , m_hideClose(false)
    , m_hideResize(false)

{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    HideResize(true);
    HideRotate(true);
}

void PencilItem::Created(){
    m_isCreating = false;
    QRectF rc = path().boundingRect();

//    setPos(0, 0);
}

void PencilItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = path().boundingRect();

    QColor color(255, 0, 0, 128);

    if (option->state & QStyle::State_Selected)
    {
        //绘制选中边框
        //if (option->state & QStyle::State_HasFocus)
        {
            QPen pen_r;
            pen_r.setColor(color);
            pen_r.setWidth(1);
            pen_r.setStyle(Qt::DashLine);
            painter->setPen(pen_r);
            //选中边框
            painter->drawRect(rc);
        }

        if(!m_hideResize)
        {
            //缩放按钮区域
            QPixmap px_m(":/resources/images/move.png");
            px_m = px_m.scaled(20, 20);
            painter->drawPixmap(rc.bottomRight()-QPointF(20, 20), px_m);
        }

        if(!m_hideRotate)
        {
            //旋转按钮区域
            QPixmap px_r(":/resources/images/rotate-left.png");
            px_r = px_r.scaled(20, 20);
            painter->drawPixmap((rc.topLeft()+rc.topRight())/2, px_r);
        }

        if(!m_hideClose)
        {
            //关闭按钮区域
            QPixmap px_c(":/resources/images/close.png");
            px_c = px_c.scaled(20, 20);
            painter->drawPixmap(rc.topRight()-QPointF(20, 0), px_c);
        }

    }

    QGraphicsPathItem::paint(painter, option, widget);
}

void PencilItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (isInResizeArea(event->pos())){
        setCursor(Qt::SizeAllCursor);
    }else if (isInRotateArea(event->pos()))
    {
        QCursor rotateCursor(QPixmap(":/resources/images/rotate-left-cur.png"));
        setCursor(rotateCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void PencilItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF rc = path().boundingRect();
    if (m_isResizing && !m_hideResize)
    {
        int dx = int(2.0 * event->pos().x());
        int dy = int(2.0 * event->pos().y());
        prepareGeometryChange();

//        m_width = dx;
//        m_height = dy;
//        if (m_width < MIN_ITEM_SIZE){
//            m_width = MIN_ITEM_SIZE;
//        }else if (m_width > MAX_ITEM_SIZE){
//            m_width = MAX_ITEM_SIZE;
//        }
//        if (m_height < MIN_ITEM_SIZE){
//            m_height = MIN_ITEM_SIZE;
//        }else if (m_height > MAX_ITEM_SIZE){
//            m_height = MAX_ITEM_SIZE;
//        }

        qDebug()<<"resize, scenePos="<<scenePos();
    }
    else if (m_isRotating && !m_hideRotate)
    {
        QPointF cursorPos = event->pos();
        QVector2D vectorStart = QVector2D(QPointF(0.0, -rc.width() / 2) - QPointF(0.0, 0.0));           // 起始向量
        QVector2D vectorEnd = QVector2D(cursorPos - QPointF(0.0, 0.0));                             // 结束向量
        // 计算起始向量和结束向量之间的角度
        qreal angle = 0.0;
        qreal angleEnd = GetDegreeAngle(vectorEnd);
        qreal angleStart = GetDegreeAngle(vectorStart);
        angle = angleEnd - angleStart + rotation();
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

//        if(m_pCBRotate){
//            QPointF _scenePos = scenePos();
//            m_pCBRotate(_scenePos.x(), _scenePos.y(), angle);
//        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void PencilItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
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

void PencilItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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
}

bool PencilItem::isInResizeArea(const QPointF &pos) const
{
    QRectF rc = path().boundingRect();
    return QRectF(rc.bottomRight()-QPointF(20, 20), rc.bottomRight()).contains(pos);
}

bool PencilItem::isInRotateArea(const QPointF &pos) const
{
    QRectF rc = path().boundingRect();
    QPointF pt = (rc.topLeft()+rc.topRight())/2;
    QRectF rcS = QRectF(pt.x(), pt.y(), 20, 20);
    //qDebug() << rcS << "--" << pt;
    return rcS.contains(pos);
}

bool PencilItem::isInCloseArea(const QPointF &pos) const
{
    QRectF rc = path().boundingRect();
    QPointF pt = rc.topRight();
    return QRectF(pt.x()-20, pt.y(), 20, 20).contains(pos);
}

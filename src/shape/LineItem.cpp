#include "LineItem.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

const int RZICON_LEN = 20;

inline QPointF rzPos(const QPointF &p1, const QPointF &p2){
    // p2与p1都在x轴
    if(p2.toPoint().y() == p1.toPoint().y()){
        if(p2.x() < p1.x()){// p2在左，p1在右
            return p2+QPointF(RZICON_LEN, 0);
        }else{// p2在右，p1在左
            return p2-QPointF(RZICON_LEN, 0);
        }
    }
    // p2与p1都在y轴
    if(p2.toPoint().x() == p1.toPoint().x()){
        if(p2.y() < p1.y()){// p2在上，p1在下
            return p2+QPointF(0, RZICON_LEN);
        }else{// p2在下，p1在上
            return p2-QPointF(0, RZICON_LEN);
        }
    }

    qreal m = RZICON_LEN;//p0与p2的距离
    qreal r = (p1.y()-p2.y())  / (p1.x()-p2.x());//斜率
    //p2-p0向量与p2-p1同方向
    //p2-p1东北方向
    qreal x0 = p2.x(), y0 = p2.y();
    if(p1.x()>p2.x() && p1.y()<p2.y()){
        x0 = sqrt(m*m/(1+r*r)) + p2.x();
        y0 = -sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()>p2.x() && p1.y()>p2.y()){//东南方向
        x0 = sqrt(m*m/(1+r*r)) + p2.x();
        y0 = sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()<p2.x() && p1.y()>p2.y()){//西南方向
        x0 = -sqrt(m*m/(1+r*r)) + p2.x();
        y0 = sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()<p2.x() && p1.y()<p2.y()){//西北方向
        x0 = -sqrt(m*m/(1+r*r)) + p2.x();
        y0 = -sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }
    return QPointF(x0, y0);
}

LineItem::LineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
    , m_pressed(false)
    , m_isResizing(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsLineItem::paint(painter, option, widget);
    if(isSelected()){
       QPen p = this->pen();
       p.setColor(Qt::blue);
       painter->setPen(p);
       QPointF p1 = line().p1();
       QPointF p2 = line().p2();
       painter->drawPoint(p1);

       QPixmap px_c(":/resources/images/close.png");
       px_c = px_c.scaled(RZICON_LEN, RZICON_LEN);
       QPointF ptClose = (p1+p2)/2;
       painter->drawPixmap(ptClose.x()-RZICON_LEN/2, ptClose.y()-RZICON_LEN/2, QPixmap(px_c));

       QPixmap px_r(":/resources/images/move.png");
       px_r = px_r.scaled(RZICON_LEN, RZICON_LEN);
       QPointF ptMove = rzPos(p1, p2);
       painter->drawPixmap(ptMove.x()-RZICON_LEN/2, ptMove.y()-RZICON_LEN/2, QPixmap(px_r));
    }
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event){

    m_pressed = true;
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInResizeArea(event->pos()) && isSelected())
    {
        m_isResizing = true;
        m_beginSizePoint = event->pos();
    }
    else if (event->button() == Qt::LeftButton && isInCloseArea(event->pos()) && isSelected())
    {
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    if(m_pressed && m_isResizing){
        QPointF deltaPt = event->pos()-m_beginSizePoint;
        QPointF p1 = line().p1();
        QPointF p2 = line().p2();
        qDebug() << "LineItem::mouseMoveEvent before p1=" <<p1<<", p2="<<p2;
        qDebug() << "LineItem::mouseMoveEvent deltaPt=" <<deltaPt;
        p2 = p2+deltaPt;
        setLine(QLineF(p1, p2));
        qDebug() << "LineItem::mouseMoveEvent after p1=" <<p1<<", p2="<<p2;
        m_beginSizePoint = event->pos();
    }
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseReleaseEvent(event);
    m_pressed = false;
    m_isResizing = false;
}

void LineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ((isInResizeArea(event->pos()) && isSelected())){
        setCursor(Qt::SizeAllCursor);
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }else{
        unsetCursor();
        setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

bool LineItem::isInResizeArea(const QPointF &pos) const
{
    QPointF p1 = line().p1();
    QPointF p2 = line().p2();
    QPointF ptMove = rzPos(p1, p2);
    bool b = QRectF(ptMove.x()-RZICON_LEN/2, ptMove.y()-RZICON_LEN/2, RZICON_LEN, RZICON_LEN).contains(pos);
    if(b){
        //qDebug() << "isInResizeArea";
    }
    return b;
}

bool LineItem::isInCloseArea(const QPointF &pos) const
{
    QPointF p1 = line().p1();
    QPointF p2 = line().p2();
    QPointF ptClose = (p1+p2)/2;
    bool b = QRectF(ptClose.x()-RZICON_LEN/2, ptClose.y()-RZICON_LEN/2, RZICON_LEN, RZICON_LEN).contains(pos);
    if(b){
        //qDebug() << "isInCloseArea";
    }
    return b;
}

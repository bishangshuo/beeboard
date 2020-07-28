#include "GraphicsLineItem.h"
#include <QPen>
#include <QPainter>

GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{

}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsLineItem::paint(painter, option, widget);
    if(isSelected()){
       QPen p = this->pen();
       p.setColor(Qt::yellow);
       painter->setPen(p);
       QPointF p1 = line().p1();
       QPointF p2 = line().p2();
       painter->drawPoint(p1);
       painter->drawPoint(p2);
    }
}

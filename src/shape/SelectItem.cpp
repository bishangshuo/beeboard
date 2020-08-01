#include "SelectItem.h"

#include <QPainter>


SelectItem::SelectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
{
}

void SelectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setColor(QColor(255,0, 0, 128));
    pen.setWidth(1);
    pen.setStyle(Qt::DashDotLine);
    painter->setPen(pen);
    painter->drawRect(rect());

    QGraphicsRectItem::paint(painter, option, widget);
}

QRectF SelectItem::boundingRect() const
{
    return rect();
}

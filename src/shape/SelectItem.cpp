#include "SelectItem.h"

#include <QPainter>


SelectItem::SelectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
{
    m_pen.setColor(QColor(255,0, 0, 128));
    m_pen.setWidth(1);
    m_pen.setStyle(Qt::DashDotLine);
}

void SelectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(m_pen);
    painter->drawRect(rect());

    QGraphicsRectItem::paint(painter, option, widget);
}

QRectF SelectItem::boundingRect() const
{
    return rect();
}

#include "RectangleItem.h"
#include <QPainter>

RectangleItem::RectangleItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{

}

void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    painter->setPen(pen);
    painter->drawRect(rc);

    ItemBase::paint(painter, option, widget);
}

ItemBase *RectangleItem::createNew(int x, int y, int width, int height)
{
    return new RectangleItem(x, y, width, height);
}

#include "RectItem.h"
#include <QPainter>

RectItem::RectItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{

}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

ItemBase *RectItem::createNew(int x, int y, int width, int height)
{
    return new RectItem(x, y, width, height);
}

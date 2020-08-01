#include "EllipseItem.h"

#include <QPainter>

EllipseItem::EllipseItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{

}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    painter->setPen(pen);
    painter->drawEllipse(rc);

    ItemBase::paint(painter, option, widget);
}

ItemBase *EllipseItem::createNew(int x, int y, int width, int height)
{
    return new EllipseItem(x, y, width, height);
}

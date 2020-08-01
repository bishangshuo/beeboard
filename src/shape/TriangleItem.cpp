#include "TriangleItem.h"

#include <QPainter>

TriangleItem::TriangleItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{

}

void TriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->drawLine(QLineF(0, -m_height/2, m_width/2, m_height/2));
    painter->drawLine(QLineF(0, -m_height/2, -m_width/2, m_height/2));
    painter->drawLine(QLineF(-m_width/2, m_height/2, m_width/2, m_height/2));

    ItemBase::paint(painter, option, widget);
}

ItemBase *TriangleItem::createNew(int x, int y, int width, int height)
{
    return new TriangleItem(x, y, width, height);
}

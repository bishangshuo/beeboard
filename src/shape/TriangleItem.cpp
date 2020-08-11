#include "TriangleItem.h"
#include "src/property/PropObj.h"
#include <QPainter>

TriangleItem::TriangleItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
    m_pen.setJoinStyle(PropObj::GetInstance()->PenJoinStyle());
    m_pen.setCapStyle(PropObj::GetInstance()->PenCapStyle());
}

void TriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);

    painter->setPen(m_pen);
    painter->drawLine(QLineF(0, -m_height/2, m_width/2, m_height/2));
    painter->drawLine(QLineF(0, -m_height/2, -m_width/2, m_height/2));
    painter->drawLine(QLineF(-m_width/2, m_height/2, m_width/2, m_height/2));

    ItemBase::paint(painter, option, widget);
}

ItemBase *TriangleItem::createNew(int x, int y, int width, int height)
{
    return new TriangleItem(x, y, width, height);
}

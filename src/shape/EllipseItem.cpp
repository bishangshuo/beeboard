#include "EllipseItem.h"

#include <QPainter>
#include "src/property/PropObj.h"

EllipseItem::EllipseItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);
    painter->setPen(m_pen);
    painter->drawEllipse(rc);

    ItemBase::paint(painter, option, widget);
}

ItemBase *EllipseItem::createNew(int x, int y, int width, int height)
{
    return new EllipseItem(x, y, width, height);
}

#include "RectangleItem.h"
#include "src/property/PropObj.h"
#include <QPainter>

RectangleItem::RectangleItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
}

void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);

    painter->setPen(m_pen);
    painter->drawRect(rc);

    ItemBase::paint(painter, option, widget);
}

ItemBase *RectangleItem::createNew(int x, int y, int width, int height)
{
    return new RectangleItem(x, y, width, height);
}

#include "MultiSelectorItem.h"

#include <QPainter>

MultiSelectorItem::MultiSelectorItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{
    //HideBaseControls(true);
}

void MultiSelectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setRenderHint(QPainter::Antialiasing, true);

//    QRectF rc = QRectF(-m_width/2, -m_height/2,  m_width, m_height);
//    QPen pen;
//    pen.setWidth(1);
//    pen.setColor(QColor(255, 0, 0, 128));
//    pen.setStyle(Qt::DashLine);
//    painter->setPen(pen);
//    painter->drawRect(rc);

    ItemBase::paint(painter, option, widget);
}

ItemBase *MultiSelectorItem::createNew(int x, int y, int width, int height)
{
    return new MultiSelectorItem(x, y, width, height);
}

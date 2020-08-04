#include "MultiSelectorItem.h"

#include <QPainter>

MultiSelectorItem::MultiSelectorItem(int x, int y, int width, int height)
    : ItemBase(x, y, width, height)
{
    HideResize(true);
    HideRotate(true);
}

void MultiSelectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ItemBase::paint(painter, option, widget);
}

ItemBase *MultiSelectorItem::createNew(int x, int y, int width, int height)
{
    return new MultiSelectorItem(x, y, width, height);
}

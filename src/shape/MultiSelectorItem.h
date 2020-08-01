#ifndef MULTISELECTORITEM_H
#define MULTISELECTORITEM_H

#include "ItemBase.h"

class MultiSelectorItem : public ItemBase
{
public:
    MultiSelectorItem(int x, int y, int width, int height);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ItemBase *createNew(int x, int y, int width, int height) override;
};

#endif // MULTISELECTORITEM_H

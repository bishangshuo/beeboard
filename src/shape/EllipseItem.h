#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H

#include "ItemBase.h"
class EllipseItem : public ItemBase
{
public:
    EllipseItem(int x, int y, int width, int height);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ItemBase *createNew(int x, int y, int width, int height) override;
};

#endif // ELLIPSEITEM_H

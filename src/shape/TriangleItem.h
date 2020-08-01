#ifndef TRIANGLEITEM_H
#define TRIANGLEITEM_H

#include "ItemBase.h"
class TriangleItem : public ItemBase
{
public:
    TriangleItem(int x, int y, int width, int height);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ItemBase *createNew(int x, int y, int width, int height) override;
};

#endif // TRIANGLEITEM_H

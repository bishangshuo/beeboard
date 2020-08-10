#ifndef RECTITEM_H
#define RECTITEM_H

#include "ItemBase.h"

class RectangleItem : public ItemBase
{
public:
    /**
     * @brief RectangleItem 构造函数
     * @param x 中心点x坐标
     * @param y 中心点y坐标
     * @param width 矩形宽
     * @param height 矩形搞
     */
    RectangleItem(int x, int y, int width, int height);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ItemBase *createNew(int x, int y, int width, int height) override;
};

#endif // RECTITEM_H

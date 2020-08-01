#ifndef RECTITEM_H
#define RECTITEM_H

#include "ItemBase.h"

class RectItem : public ItemBase
{
public:
    /**
     * @brief RectItem 构造函数
     * @param x 中心点x坐标
     * @param y 中心点y坐标
     * @param width 矩形宽
     * @param height 矩形搞
     */
    RectItem(int x, int y, int width, int height);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    ItemBase *createNew(int x, int y, int width, int height) override;
};

#endif // RECTITEM_H

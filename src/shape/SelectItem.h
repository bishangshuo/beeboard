#ifndef SELECTITEM_H
#define SELECTITEM_H

#include <QGraphicsRectItem>

class SelectItem : public QGraphicsRectItem
{
public:
    SelectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);

protected:

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // SELECTITEM_H

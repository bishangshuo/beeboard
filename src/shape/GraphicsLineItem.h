#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include <QGraphicsLineItem>

class GraphicsLineItem : public QGraphicsLineItem
{
public:
    GraphicsLineItem(QGraphicsItem *parent = nullptr);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

#endif // GRAPHICSLINEITEM_H

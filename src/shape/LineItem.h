#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>
#include "src/shape/ItemCtrl.h"

class LineItem : public QGraphicsLineItem, public ItemCtrl
{
public:
    LineItem(QGraphicsItem *parent = nullptr);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    bool isInResizeArea(const QPointF &pos) const;
    bool isInCloseArea(const QPointF &pos) const;
private:
    bool m_pressed;
    bool m_isResizing;
    QPointF m_beginSizePoint;
};

#endif // LINEITEM_H

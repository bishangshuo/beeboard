#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>
#include "src/shape/ItemCtrl.h"
#include <QPen>
#include <QBrush>

class LineItem : public QGraphicsLineItem, public ItemCtrl
{
public:
    LineItem(QGraphicsItem *parent = nullptr);
    ~LineItem();

    void HideClose(bool hide){
        m_hideClose = hide;
    }

    void HideResize(bool hide){
        m_hideResize = hide;
    }

    void SetPen(QPen pen){
        m_pen = pen;
    }

    void SetBrush(QBrush brush){
        m_brush = brush;
    }

    QPen GetPen() const{
        return m_pen;
    }

    QBrush GetBrush() const {
        return m_brush;
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;

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

    bool m_hideClose;
    bool m_hideResize;

    QPainterPath m_pathShape;

    QPen m_pen;
    QBrush m_brush;
};

#endif // LINEITEM_H

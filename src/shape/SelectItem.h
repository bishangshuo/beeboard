#ifndef SELECTITEM_H
#define SELECTITEM_H

#include <QPen>
#include <QBrush>
#include <QGraphicsRectItem>

class SelectItem : public QGraphicsRectItem
{
public:
    SelectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);

    QPen GetPen() const{
        return m_pen;
    }
    QBrush GetBrush() const{
        return m_brush;
    }
protected:

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPen m_pen;
    QBrush m_brush;
};

#endif // SELECTITEM_H

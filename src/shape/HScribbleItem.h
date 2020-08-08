#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QPainter>

class HScribbleItem : public QGraphicsRectItem
{
public:
    HScribbleItem(QGraphicsItem *parent = nullptr);
    ~HScribbleItem();
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override{
        return rect();
    }
public:
    void SetPixmap(const QPixmap &pixmap);
private:
    QPixmap mPixmap;
    QPointF mPt;

    long m_curPathKey;
};

#endif // GRAPHICSRECTITEM_H

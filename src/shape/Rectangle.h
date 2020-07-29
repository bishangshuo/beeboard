#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ShapeBase.h"
class QGraphicsRectItem;

class Rectangle : public ShapeBase
{
    Q_OBJECT
public:
    Rectangle(QObject *parent = nullptr);
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void SetSelected() override;
    void Remove(GraphicsScene *pScene) override;

    void RotateBegin() override;
    void Rotate(qreal angle) override;
    void RotateEnd() override;

    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;
private:
    QGraphicsRectItem *m_pItem;
};

#endif // RECTANGLE_H

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "ShapeBase.h"

class QGraphicsEllipseItem;

class Ellipse : public ShapeBase
{
public:
    Ellipse(QObject *parent = nullptr);
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void SetSelected() override;
    void Remove(GraphicsScene *pScene) override;

    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;
private:
    QGraphicsEllipseItem *m_pItem;
};

#endif // ELLIPSE_H

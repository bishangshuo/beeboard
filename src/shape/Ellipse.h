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
private:
    QGraphicsEllipseItem *m_pItem;
};

#endif // ELLIPSE_H

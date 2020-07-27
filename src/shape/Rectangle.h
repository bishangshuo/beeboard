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

    QRect GetRect() override;
private:
    QGraphicsRectItem *m_pItem;
};

#endif // RECTANGLE_H

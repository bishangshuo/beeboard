#ifndef LINE_H
#define LINE_H

#include "ShapeBase.h"
class QGraphicsLineItem;

class Line : public ShapeBase
{
public:
    Line(QObject *parent = nullptr);
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void SetSelected() override;
    void Remove(GraphicsScene *pScene) override;

    QRect GetRect() override;
private:
    QGraphicsLineItem *m_pItem;
};

#endif // LINE_H

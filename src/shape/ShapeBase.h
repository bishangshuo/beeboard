#ifndef SHAPEBASE_H
#define SHAPEBASE_H

#include <QObject>

class GraphicsScene;
class QGraphicsItem;

const int ITEM_DATA_KEY =0x8001;

class ShapeBase : public QObject
{
    Q_OBJECT
public:
    explicit ShapeBase(QObject *parent = nullptr);
    virtual ~ShapeBase();

    virtual int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) = 0;
    virtual void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) = 0;
    virtual void SetSelected() = 0;
    virtual void Remove(GraphicsScene *pScene) = 0;

    virtual QRect GetRect() = 0;
    virtual QPointF GetP1() = 0;
    virtual QPointF GetP2() = 0;
    virtual QPointF GetPos() = 0;
    virtual void ChangePos(qreal dx, qreal dy) = 0;
    virtual QGraphicsItem *GetGraphicsItem() = 0;
    virtual void ChangeSize(qreal dx, qreal dy) = 0;

signals:

};

#endif // SHAPEBASE_H

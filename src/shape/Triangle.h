#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ShapeBase.h"
class TriangleItem;

class Triangle : public ShapeBase
{
    Q_OBJECT
public:
    Triangle(QObject *parent = nullptr);
    ~Triangle();
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void CreateEnd(const QPointF &pos, GraphicsScene *pScene) override;
    void SetSelected(bool selected) override;
    void SetEditable(bool editable) override;
    void Remove(GraphicsScene *pScene) override;

    void RotateBegin() override;
    void Rotate(qreal x, qreal y, qreal angle) override;
    void RotateEnd() override;

    int GetItemKey() const override;
    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;

    void SetPen(QPen pen) override;
    void SetBrush(QBrush brush) override;
    QPen GetPen() const override;
    QBrush GetBrush() const override;

    QPixmap GetPixmap() const override{
        return QPixmap();
    }

    void HideControls(bool hide) override;
private:
    TriangleItem *m_pItem;
};

#endif // TRIANGLE_H

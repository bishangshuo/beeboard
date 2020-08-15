#ifndef SELECT_H
#define SELECT_H

#include "src/shape/ShapeBase.h"

class QGraphicsRectItem;
class SelectItem;

class Select : public ShapeBase
{
    Q_OBJECT
public:
    Select(QObject *parent = nullptr);
    ~Select();
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void CreateEnd(GraphicsScene *pScene) override;
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
    qreal GetAngle() const override{
        return 0.0;
    }
    int GetItemWidth() const override{
        return 0;
    }
    int GetItemHeight() const override{
        return 0;
    }
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;

    void SetPen(QPen pen) override{}
    void SetBrush(QBrush brush) override{}
    QPen GetPen() const override;
    QBrush GetBrush() const override;

    QPixmap GetPixmap() const override{
        return QPixmap();
    }

    void HideControls(bool hide) override;
private:
    SelectItem *m_pItem;
};

#endif // SELECT_H

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "ShapeBase.h"

class EllipseItem;

class Ellipse : public ShapeBase
{
public:
    Ellipse(QObject *parent = nullptr);
    ~Ellipse();
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
    qreal GetAngle() const override;
    int GetItemWidth() const override;
    int GetItemHeight() const override;
    QGraphicsItem *GetGraphicsItem() override;

    void SetPen(QPen pen) override;
    void SetBrush(QBrush brush) override;
    QPen GetPen() const override;
    QBrush GetBrush() const override;

    QPixmap GetPixmap() const override{
        return QPixmap();
    }

    void HideControls(bool hide) override;

    void Undo() override;
    void Redo() override;
    void UndoEraser(int eraser) override;
    void RedoEraser(int eraser) override;
private:
    EllipseItem *m_pItem;
};

#endif // ELLIPSE_H

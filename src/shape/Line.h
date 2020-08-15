#ifndef LINE_H
#define LINE_H

#include "ShapeBase.h"
class LineItem;

class Line : public ShapeBase
{
public:
    Line(QObject *parent = nullptr);
    ~Line();

    void SetPos(const QPointF &pos);
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
    void ClearRedo() override;
private:
    LineItem *m_pItem;
};

#endif // LINE_H

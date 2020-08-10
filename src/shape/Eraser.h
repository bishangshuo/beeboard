#ifndef ERASER_H
#define ERASER_H

#include "ShapeBase.h"
class Eraser : public ShapeBase
{
public:
    Eraser(QObject *parent = nullptr);

    void setWidth(int width){
        m_width = width;
    }
    int Width() const{
        return m_width;
    }
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

    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;

    void HideControls(bool hide) override;
private:
    int m_width;
};

#endif // ERASER_H

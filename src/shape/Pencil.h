#ifndef PENCIL_H
#define PENCIL_H

#include "ShapeBase.h"
#include <QPainterPath>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "src/shape/PencilItem.h"

class Eraser;

class Pencil : public ShapeBase
{
    Q_OBJECT
public:
    Pencil(QObject *parent = nullptr);
    ~Pencil();

    QPointF MapFromScene(const QPointF &p);
    int LoadFromPixmap(const QPixmap &pixmap, const QRect &rect, GraphicsScene *pScene);
signals:
    void sigShouldRemovePencil(int key);
public slots:
    void slotEraserPressed();
    void slotEraserMove(const QPointF &prevPos, const QPointF &pos, Eraser *eraser);
    void slotEraserRelease();
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

    QPixmap GetPixmap() const override;

    void HideControls(bool hide) override;
private:
    PencilItem *m_pItem;
    QPainterPath m_path;
    QPointF m_point;
};

#endif // PENCIL_H

#ifndef PENCIL_H
#define PENCIL_H

#include "ShapeBase.h"
#include <QPainterPath>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "PencilImageItem.h"

class PencilItem;

class Pencil : public ShapeBase
{
    Q_OBJECT
public:
    Pencil(QObject *parent = nullptr);
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
    PencilItem *m_pItem;
    QPainterPath m_path;
    QPixmap m_pixmap;
    QPointF m_point;
    PencilImageItem *m_pPixmapItem;
};

#endif // PENCIL_H

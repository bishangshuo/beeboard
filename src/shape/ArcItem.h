#ifndef ARCITEM_H
#define ARCITEM_H
#include "src/shape/ItemCtrl.h"
#include <QPen>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QVector2D>
#include <cmath>

const qreal PI = 3.141592653;
const qreal AnglePerPI = 180.0 / PI;
class ArcItem : public QGraphicsEllipseItem, public ItemCtrl
{
public:
    ArcItem(int x, int y, int width, int height);

    bool IsCreating() const{
        return m_isCreating;
    }

    void HideRotate(bool hide){
        m_hideRotate = hide;
    }

    void HideClose(bool hide){
        m_hideClose = hide;
    }

    int Width() const{
        return m_rcOrg.width();
    }

    int Height() const{
        return m_rcOrg.height();
    }

    void SetPen(QPen pen){
        m_pen = pen;
    }

    void SetBrush(QBrush brush){
        m_brush = brush;
    }

    QPen GetPen() const{
        return m_pen;
    }

    QBrush GetBrush() const {
        return m_brush;
    }

    void Created();
    void SetPos(const QPointF &pos);

    void Undo();
    void Redo();

    void Rotate(qreal angle);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    int type() const override;
    bool isInCloseArea(const QPointF &pos) const;
    bool isInRotateArea(const QPointF &pos) const;

    inline qreal GetDegreeAngle(QVector2D vector2d) const
    {
        return fmod((atan2((qreal)vector2d.y(), (qreal)vector2d.x()) * AnglePerPI + 360.0), 360.0 );
    }
private:
    QRectF m_rcOrg;
    bool        m_isRotating;
    bool        m_isCreating;

    bool m_hideRotate;
    bool m_hideClose;

    QPen m_pen;
    QBrush m_brush;

    qreal m_dx;
    qreal m_dy;

};

#endif // ARCITEM_H

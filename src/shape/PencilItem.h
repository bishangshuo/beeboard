#ifndef PENCILITEM_H
#define PENCILITEM_H

#include <QGraphicsPathItem>
#include <QVector2D>
#include "src/shape/ItemCtrl.h"

const qreal PI = 3.141592653;
const qreal AnglePerPI = 180.0 / PI;

class PencilItem : public QGraphicsPathItem, public ItemCtrl
{
public:
    PencilItem(QGraphicsItem *parent = nullptr);

    bool IsCreating() const{
        return m_isCreating;
    }

    void Created();

    void HideRotate(bool hide){
        m_hideRotate = hide;
    }

    void HideClose(bool hide){
        m_hideClose = hide;
    }

    void HideResize(bool hide){
        m_hideResize = hide;
    }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    bool isInResizeArea(const QPointF &pos) const;
    bool isInRotateArea(const QPointF &pos) const;
    bool isInCloseArea(const QPointF &pos) const;

    inline qreal GetDegreeAngle(QVector2D vector2d) const
    {
        return fmod((atan2((qreal)vector2d.y(), (qreal)vector2d.x()) * AnglePerPI + 360.0), 360.0 );
    }

private:
    QPainterPath mPath;

    bool        m_isResizing;
    bool        m_isRotating;
    bool        m_isCreating;

    bool m_hideRotate;
    bool m_hideClose;
    bool m_hideResize;

};

#endif // PENCILITEM_H

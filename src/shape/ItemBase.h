#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsObject>
#include <QTime>
#include <QVector2D>
#include <cmath>
#include <QPen>
#include <QBrush>
#include "src/shape/ItemCtrl.h"
#include <QStack>

const qreal PI = 3.141592653;
const qreal AnglePerPI = 180.0 / PI;

class QGraphicsScene;

class ItemBase : public QGraphicsObject, public ItemCtrl
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    ItemBase(int x, int y, int width, int height);
    ~ItemBase() override;

    void UpdateSize(int width, int height){
        m_width = width;
        m_height = height;
        update();
    }

    int Width() const{
        return m_width;
    }

    int Height() const{
        return m_height;
    }

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

    void Rotate(qreal angle);

    void Undo();
    void Redo();

signals:
    void sigItemChanged();

protected:
    virtual ItemBase *createNew(int x, int y, int width, int height) = 0;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    int type() const override;
    bool isInResizeArea(const QPointF &pos) const;
    bool isInRotateArea(const QPointF &pos) const;
    bool isInCloseArea(const QPointF &pos) const;

    inline qreal GetDegreeAngle(QVector2D vector2d) const
    {
        return fmod((atan2((qreal)vector2d.y(), (qreal)vector2d.x()) * AnglePerPI + 360.0), 360.0 );
    }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public:

    int         m_width;
    int         m_height;
    bool        m_isResizing;
    bool        m_isRotating;
    bool        m_isCreating;

    bool m_hideRotate;
    bool m_hideClose;
    bool m_hideResize;

    QPen m_pen;
    QBrush m_brush;

    qreal m_dx;
    qreal m_dy;
};


#endif // ITEMBASE_H

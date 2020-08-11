#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QGraphicsItem>
#include <QTime>
#include <QVector2D>
#include <cmath>
#include <QPen>
#include <QBrush>
#include "src/shape/ItemCtrl.h"

const qreal PI = 3.141592653;
const qreal AnglePerPI = 180.0 / PI;

class QGraphicsScene;

class ItemBase : public QGraphicsItem, public ItemCtrl
{
public:
    enum { Type = UserType + 1 };

    ItemBase(int x, int y, int width, int height);
    ~ItemBase() override;

    void UpdateSize(int width, int height){
        m_width = width;
        m_height = height;
        update();
    }

    bool IsCreating() const{
        return m_isCreating;
    }

    void Created(){
        m_isCreating = false;
    }

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

protected:
    virtual ItemBase *createNew(int x, int y, int width, int height) = 0;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    int type() const override;
    bool isInResizeArea(const QPointF &pos) const;
    bool isInRotateArea(const QPointF &pos) const;
    bool isInCloseArea(const QPointF &pos) const;

    static void duplicateSelectedItems(QGraphicsScene *scene);
    static void deleteSelectedItems(QGraphicsScene *scene);
    static void growSelectedItems(QGraphicsScene *scene);
    static void shrinkSelectedItems(QGraphicsScene *scene);

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
};


#endif // ITEMBASE_H

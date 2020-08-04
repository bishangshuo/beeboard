#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include "src/common/ToolType.h"
#include "src/shape/ShapeBase.h"
#include "src/shape/Select.h"

class GraphicsView;

typedef struct _SHAPE_DATA{
    _SHAPE_DATA(int _key, TOOL_TYPE::Type _toolType, ShapeBase *_shape, int _index)
        : key(_key), toolType(_toolType), shape(_shape), index(_index){}
    int key;
    TOOL_TYPE::Type toolType;
    ShapeBase *shape;
    int index;
}SHAPE_DATA;

typedef QMap<int, SHAPE_DATA *> MapShape;

typedef QList<int> ListShapeKey;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = nullptr);
    ~GraphicsScene(){
    }
    void setToolType(const TOOL_TYPE::Type &toolType);
    void setView(GraphicsView *view);
    void clearScene();
    void onItemPosChanged(int key, qreal dx, qreal dy);
    void onItemRemove(int key);
    void onItemsRemoveByRubberBand();
    void onMultiSelectedItemsRotate(qreal anchor_x, qreal anchor_y, qreal angle);

    void onItemResizeBegin(int key);
    void onItemResize(int key, qreal dx, qreal dy);
    void onItemResizeEnd(int key);

    void onItemRotateBegin(int key);
    void onItemRotate(int key, qreal angle);
    void onItemRotateEnd(int key);

    TOOL_TYPE::Type GetPoints(int key, QPoint &p1, QPoint &p2);
    QPoint GetDeltaPos(int key);
    qreal GetAngle(int key);

    void UnselectedAll();

    QSizeF GetViewSize() const;
signals:
    void sigSceneClicked();
    void sigItemSelected(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2);
    void sigItemResizeCompleted(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2);
    void sigItemPointsChanged(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void SetAllEditable(bool editable);
    bool onMouseSelectItem(const QPointF &pos);
    void signalItemSelected(int key);
    void SelectItemsByRubberBand(const QRectF &rubberBandRect);
    void deleteSelectItem();
    void createMultiSelector(const QRectF &rc);
    void destroyMultiSelector();
    void resetMultiSelector();
private:
    TOOL_TYPE::Type m_eToolType;
    bool m_bPressed;
    QPointF m_ptPrev;
    MapShape m_mapShape;
    int m_nCurKey;
    GraphicsView *m_pView;

    QPointF m_ptSelect;
    ShapeBase *m_pSelect;
    ShapeBase *m_multiSelector;

    ListShapeKey m_listSelectedItems;
};

#endif // GRAPHICSSCENE_H

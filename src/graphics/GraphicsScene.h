#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include "src/common/ToolType.h"
#include "src/shape/ShapeBase.h"

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
    void onItemResizeBegin(int key);
    void onItemResize(int key, qreal dx, qreal dy);
    void onItemResizeEnd(int key);
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
    void onMouseSelectItem(const QPointF &pos);
private:
    TOOL_TYPE::Type m_eToolType;
    bool m_bPressed;
    QPointF m_ptPrev;
    MapShape m_mapShape;
    int m_nCurKey;
    GraphicsView *m_pView;
};

#endif // GRAPHICSSCENE_H

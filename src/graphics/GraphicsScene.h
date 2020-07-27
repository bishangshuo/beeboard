#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include "src/common/ToolType.h"
#include "src/shape/ShapeBase.h"

class GraphicsView;

typedef struct _SHAPE_DATA{
    _SHAPE_DATA(int _key, ShapeBase *_shape, int _index)
        : key(_key), shape(_shape), index(_index){}
    int key;
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
signals:
    void sigItemSelected(int key, const QRect &rc);
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

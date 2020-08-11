#ifndef PENCILITEM_H
#define PENCILITEM_H

#include <QGraphicsPathItem>
#include <QVector2D>
#include "src/shape/ItemCtrl.h"

typedef struct _ERASER_PATH{
    _ERASER_PATH(QPainterPath *_path, int _width){
        path = _path;
        width = _width;
    }
    QPainterPath *path;
    int width;
}ERASER_PATH;

typedef QVector<ERASER_PATH *> VectorPath;
typedef QMap<int, VectorPath *> MapEraserPath;

typedef std::function<void(int)> CBPencilRemove;

class PencilItem : public QGraphicsPathItem, public ItemCtrl
{
public:
    PencilItem(QGraphicsItem *parent = nullptr);
    ~PencilItem();

    bool IsCreating() const{
        return m_isCreating;
    }

    void Created();

    void HideClose(bool hide){
        m_hideClose = hide;
    }

    void setShouldRemoveCallback(const CBPencilRemove &callback){
        m_cbRemove = callback;
    }
    void setEraser(int key, QPainterPath *path, int width);
    void onEraserRelease();

    QPainterPath shape() const override;
//    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    bool isInCloseArea(const QPointF &pos) const;

    void RenderPathToPixmap();
    void RenderEraserToPixmap();

public:
    MapEraserPath m_mapEraserPathUndo;
    MapEraserPath m_mapEraserPathRedo;

private:
    QPainterPath mPath;
    bool m_isCreating;
    bool m_hideClose;
    CBPencilRemove m_cbRemove;

    QPixmap m_pixmap;
};

#endif // PENCILITEM_H

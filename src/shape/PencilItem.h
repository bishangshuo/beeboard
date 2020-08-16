#ifndef PENCILITEM_H
#define PENCILITEM_H

#include <QGraphicsPathItem>
#include <QVector2D>
#include <QPen>
#include <QBrush>
#include "src/shape/ItemCtrl.h"

typedef struct _ERASER_PATH{
    _ERASER_PATH(QPainterPath *_path, int _width){
        path = _path;
        width = _width;
        drawn = false;
    }
    QPainterPath *path;
    int width;
    bool drawn;
}ERASER_PATH;

typedef QVector<ERASER_PATH *> VectorPath;
typedef QMap<int, VectorPath *> MapEraserPath;

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

    void setEraser(int key, QPainterPath *path, int width);
    void onEraserRelease();

    QPainterPath shape() const override;
//    QRectF boundingRect() const override;

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

    QPixmap GetPixmap() const {
        return m_pixmap;
    }

    void PencilLoaded() {
        RenderPathToPixmap();
    }

    void Undo();
    void Redo();

    void UndoEraser(int eraser);
    void RedoEraser(int eraser);

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

    QPixmap m_pixmap;
    QPixmap m_pmPath;

    QPen m_pen;
    QBrush m_brush;
};

#endif // PENCILITEM_H

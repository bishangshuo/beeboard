#ifndef PENCIL_H
#define PENCIL_H

#include "ShapeBase.h"
#include <QPainterPath>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "src/shape/PencilItem.h"

typedef struct _ERASER_DATA{
    _ERASER_DATA(QByteArray *_ba, int _width)
        : ba(_ba), width(_width){}
    QByteArray *ba;
    int width;
}ERASER_DATA;

class Eraser;

class Pencil : public ShapeBase
{
    Q_OBJECT
public:
    Pencil(QObject *parent = nullptr);
    ~Pencil();

    QPointF MapFromScene(const QPointF &p);
    int LoadFromPixmap(const QPixmap &pixmap, const QRect &rect, GraphicsScene *pScene);

    //获取path序列化后的数据，由外部分配内存，返回数据长度
    int GetPathData(QByteArray *ba) const;

    //获取橡皮擦列表数据，由函数内部分配内存，使用一定要释放内存
    QList<ERASER_DATA> *GetEraserData() const;
    //释放已经使用的内存
    void SafeDelete(QList<ERASER_DATA> *listEraser);

    void SetPath(const QPainterPath &path);
    void AddEraserPath(const QPainterPath &path, int width);
    void SetPos(const QPointF &pos);

    void Repaint() {
        m_pItem->update();
    }
signals:
    void sigShouldRemovePencil(int key);
    void sigEraserAttach(int key);
public slots:
    void slotEraserPressed();
    void slotEraserMove(const QPointF &prevPos, const QPointF &pos, Eraser *eraser);
    void slotEraserRelease();
protected:
    int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void CreateEnd(GraphicsScene *pScene) override;
    void SetSelected(bool selected) override;
    void SetEditable(bool editable) override;
    void Remove(GraphicsScene *pScene) override;

    void RotateBegin() override;
    void Rotate(qreal x, qreal y, qreal angle) override;
    void RotateEnd() override;

    int GetItemKey() const override;
    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    qreal GetAngle() const override{
        return 0.0;
    }
    int GetItemWidth() const override{
        return 0;
    }
    int GetItemHeight() const override{
        return 0;
    }
    QGraphicsItem *GetGraphicsItem() override;

    void SetPen(QPen pen) override;
    void SetBrush(QBrush brush) override;
    QPen GetPen() const override;
    QBrush GetBrush() const override;

    QPixmap GetPixmap() const override;

    void HideControls(bool hide) override;

    void Undo() override;
    void Redo() override;
    void UndoEraser(int eraser) override;
    void RedoEraser(int eraser) override;
private:
    PencilItem *m_pItem;
    QPainterPath m_path;
    QPointF m_point;
    bool m_bInfectedByEraser;
};

#endif // PENCIL_H

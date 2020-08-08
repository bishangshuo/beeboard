#ifndef HPENCIL_H
#define HPENCIL_H

#include "ShapeBase.h"
#include "src/graphics/GraphicsScene.h"
#include "HScribbleItem.h"
#include "wbcanvasitem.h"
//#include "heraser.h"

typedef QVector<QPainterPath *> VectorPath;
typedef QMap<long, VectorPath *> MapEraserPath;


typedef QList<QRectF> ListItemRect;
typedef QList<qreal> ListItemArc;
typedef QList<qreal> ListAngle;

class HPencil : public ShapeBase
{
    Q_OBJECT
public:
    HPencil(QObject *parent = nullptr);
    ~HPencil();
    QPixmap GetPixmap() const{
        return mPixmap;
    }

    //qlonglong LoadFromPixmap(const QPixmap &pixmap, const QRect &rect, GraphicsScene *scene);
signals:
    void sigEraseComplete(const qlonglong &itemKey);
    void sigShouldRemovePencil(const qlonglong &itemKey);
public:
    int Create(const QPointF &beginPos, const QPointF &endPos, GraphicsScene *scene) override;
    void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) override;
    void CreateEnd(const QPointF &pos, GraphicsScene *pScene) override;
    void SetSelected(bool selected) override;
    void SetEditable(bool editable) override;
    void Remove(GraphicsScene *pScene) override;
    void RotateBegin() override;
    void Rotate(qreal x, qreal y, qreal angle) override;
    void RotateEnd() override;

    QRect GetRect() override;
    QPointF GetP1() override;
    QPointF GetP2() override;
    QPointF GetPos() override;
    void ChangePos(qreal dx, qreal dy) override;
    QGraphicsItem *GetGraphicsItem() override;
    void ChangeSize(qreal dx, qreal dy) override;

    void HideControls(bool hide) override;

public slots:
//    void slotEraserMove(const QPointF &prevPos, const QPointF &pos, HEraser *eraser);
//    void slotEraserRelease(const QPointF &pos, HEraser *eraser);
//    void slotUndoEraser(HEraser *eraser);
//    void slotRedoEraser(HEraser *eraser);
//    void slotClearRedo();
private:
    QPointF MapFromScene(const QPointF &p);
    void SaveOrgPixmap(const QPixmap &pixmap);
    //QPixmap MixEraser(const QPixmap &pixmap);
    void RenderPath(const QPainterPath &path, GraphicsScene *scene);
    void RenderPixmap(const QPixmap &pixmap, const QRect &rc, GraphicsScene *scene);
    qreal TrimAngle(qreal angle);
private:
//    HScribbleItem *m_pItem;
    WbCanvasItem *m_pItem;
    QPainterPath mPath;
    QPainterPath mPathRotate;
    bool bPress;
    bool bCreated;
    QPixmap mPixmap;
    QPixmap *m_pOrgPixmap;
    QPointF ptBegin;
    QList<QPointF> mListPoint;
    QPainterPath mPathPre;
    QPainterPath mPathCur;
    QPainterPath mPathSub;
    QRect m_rcLayer;

    MapEraserPath m_mapEraserPathUndo;
    MapEraserPath m_mapEraserPathRedo;

    qreal m_rRotateAng;
    QPointF m_ptCenter;


    QPointF m_nBeginPos;
    QPointF m_nEndPos;
    bool m_bSelected;

    QPen *m_pPen;
    QBrush *m_pBrush;

    //每个对象都有自己位置和角度变化的轨迹，以便回滚
    ListItemRect m_listRect;
    //旋转过程中的角度轨迹
    //ListItemArc m_listAngle;

    ListAngle m_listUndoAngle;
    ListAngle m_listRedoAngle;

    qreal m_rAngle;
};

#endif // HPENCIL_H

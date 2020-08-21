#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QStack>
#include <QThread>
#include "src/common/ToolType.h"
#include "src/shape/ShapeBase.h"
#include "src/shape/Select.h"
#include "src/shape/pbshape.pb.h"
#include "src/docman/HDocIo.h"

class Eraser;
class GraphicsView;

typedef struct _SHAPE_DATA{
    _SHAPE_DATA(int _key, TOOL_TYPE::Type _toolType, ShapeBase *_shape, int _index)
        : key(_key), toolType(_toolType), shape(_shape), index(_index), isActivated(true){}
    int key;
    TOOL_TYPE::Type toolType;
    ShapeBase *shape;
    int index;
    bool isActivated;
}SHAPE_DATA;

typedef QMap<int, SHAPE_DATA *> MapShape;
typedef QList<int> ListShapeKey;

enum action_type{
    act_type_null = 1,
    act_type_create_item,
    act_type_change_geo,
    act_type_remove_item,
    act_type_erase
};

typedef struct _ACTION_NODE{
    _ACTION_NODE(){}
    _ACTION_NODE(const _ACTION_NODE &node)
        : action(node.action)
        , shape(node.shape){}

    _ACTION_NODE(action_type _act, ShapeBase *_shape)
        : action(_act)
        , shape(_shape){}
    action_type action;
    ShapeBase *shape;
}ACTION_NODE;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = nullptr);
    ~GraphicsScene();
    void setToolType(const TOOL_TYPE::Type &toolType);
    void setView(GraphicsView *view);
    void clearScene();
    void destroyRedoShapes();
    void onItemRemove(int key, bool destroy = false);
    void onItemGeoChanged(int key);
    void onItemsRemoveByRubberBand();
    void onMultiSelectedItemsRotate(qreal anchor_x, qreal anchor_y, qreal angle);

    TOOL_TYPE::Type GetPoints(int key, QPoint &p1, QPoint &p2);
    QPoint GetDeltaPos(int key);
    qreal GetAngle(int key);
    void UnselectedAll();
    QSizeF GetViewSize() const;

    void Undo();
    void Redo();

    void UndoEraser(Eraser *eraser);
    void RedoEraser(Eraser *eraser);

    //protobuf操作
    void SaveItemToProtobuf(ShapeBase *shape);
    void ModifyItemInProtobuf(ShapeBase *shape);
    void RemoveItemFromProtobuf(ShapeBase *shape);
    //save protobuf to file
    void SaveProtobufToFile();
    //load items from protobuf file
    void LoadSceneFromFile(const QString &fileName);
    void LoadLineObject(const PBShape::Line &line);
    void LoadRectangleObject(const PBShape::Rectangle &rectangle);
    void LoadEllipseObject(const PBShape::Ellipse &ellipse);
    void LoadTriangleObject(const PBShape::Triangle &triangle);
    void LoadScribbleObject(const PBShape::Scribble &scribble);

    /**
     * @brief BeginDrawLine
     * @param pos
     * @return
     * 开始画直线
     * 返回直线key
     */
    int BeginDrawLine(const QPointF &pos);

    /**
     * @brief UpdateLine
     * @param key BeginDrawLine返回的key
     * @param pos
     * 实时更新线段末端点坐标
     */
    void UpdateLine(int key, const QPointF &pos);

    /**
     * @brief EndDrawLine
     * @param key BeginDrawLine返回的key
     * @param pos
     * 停止绘制直线
     */
    void EndDrawLine(int key, const QPointF &pos);

    /**
     * @brief BeginDrawArc
     * @param rc  圆弧所在的外接正方向rect
     * @param startAngle 原点O与起始点P连接线向量OP，OP相对于向右方向的角度startAngle, 0 <= startAngle < 360
     *                  当startAngle >= 360, startAngle = startAngle % 360
     * @return
     * 开始画弧
     */
    int BeginDrawArc(const QRectF &rc, int startAngle);

    /**
     * @brief UpdateArc
     * @param key
     * @param spanAngle
     * 鼠标拖动过程中，鼠标坐标点Q，向量OQ与OP形成的夹角spanAngle, -720 < spanAngle < 720
     * 当 spanAngle >= 720, spanAngle = spanAngle % 720, 当 spanAngle <= -720, spanAngle = -abs(spanAngle) % 720
     */
    void UpdateArc(int key, int spanAngle);

    /**
     * @brief EndDrawArc
     * @param key
     * 停止绘制弧线
     */
    void EndDrawArc(int key);

signals:
    void sigSceneClicked();

    void sigEraserPressed();
    void sigEraserMove(const QPointF &prePos, const QPointF &pos, Eraser *eraser);
    void sigEraserRelease();

    void sigUndoAvailable(bool available);
    void sigRedoAvailable(bool available);

    void sigWriteSceneObject(const char *data, int size, const QString &fileName);
public slots:
    void slotWriteSceneObjectCompleted(bool res, const char *data, const QString &fileName);
    void slotEraserAttachToPencil(int key);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void MakeFileName();
    void SetAllEditable(bool editable);
    bool onMouseSelectItem(const QPointF &pos);
    void SelectItemsByRubberBand(const QRectF &rubberBandRect);
    void deleteSelectItem();
    void createMultiSelector(const QRectF &rc);
    void destroyMultiSelector();
    void resetMultiSelector();
    void onSelectedItemsChanged();
    void OnSceneChanged();
    void doClearScene();

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

    QStack<ACTION_NODE> m_stUndo;
    QStack<ACTION_NODE> m_stRedo;

    PBShape::Scene *m_pScenePb;
    HDocIo m_ioSceneObj;
    QThread m_thSceneObj;

    QString m_strSceneObjectFileName;
};

#endif // GRAPHICSSCENE_H

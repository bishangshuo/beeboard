#ifndef SHAPEBASE_H
#define SHAPEBASE_H

#include <QObject>
#include <QPen>
#include "src/property/PropObj.h"
#include "src/common/ToolType.h"

class GraphicsScene;
class QGraphicsItem;

const int ITEM_DATA_KEY = 0x8001;

class ShapeBase : public QObject
{
    Q_OBJECT
public:
    explicit ShapeBase(QObject *parent = nullptr);
    virtual ~ShapeBase();

    virtual int Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) = 0;
    virtual void UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) = 0;
    virtual void CreateEnd(GraphicsScene *pScene) = 0;
    virtual void SetSelected(bool selected) = 0;
    virtual void SetEditable(bool editable) = 0;
    virtual void Remove(GraphicsScene *pScene) = 0;
    virtual void RotateBegin() = 0;
    virtual void Rotate(qreal x, qreal y, qreal angle) = 0;
    virtual void RotateEnd() = 0;

    virtual int GetItemKey() const = 0;
    virtual QRect GetRect() = 0;
    virtual QPointF GetP1() = 0;
    virtual QPointF GetP2() = 0;
    virtual QPointF GetPos() = 0;
    virtual qreal GetAngle() const = 0;
    virtual int GetItemWidth() const = 0;
    virtual int GetItemHeight() const = 0;
    virtual QGraphicsItem *GetGraphicsItem() = 0;

    virtual void SetPen(QPen pen) = 0;
    virtual void SetBrush(QBrush brush) = 0;
    virtual QPen GetPen() const = 0;
    virtual QBrush GetBrush() const = 0;
    virtual QPixmap GetPixmap() const = 0;
    virtual void HideControls(bool hide) = 0;

    virtual void Undo() = 0;
    virtual void Redo() = 0;
    virtual void ClearRedo() = 0;

public:
    inline qreal trimAngle(qreal angle){
        qreal ang = angle;
        if (ang > 360.0)
        {
            while(1)
            {
                ang -= 360.0;
                if (ang < 360.0) break;
            }
        }
        else if (ang < 0.0)
        {
            while(1)
            {
                ang += 360.0;
                if (ang > 0.0) break;
            }
        }
        return ang;
    }

    TOOL_TYPE::Type GetType() const {
        return m_type;
    }

signals:
    void sigRemove(int key);
    void sigRotate(qreal anchor_x, qreal anchor_y, qreal angle);
    void sigGeoChanged(int key);
protected:
    qreal m_rAngle;
    TOOL_TYPE::Type m_type;
};

#endif // SHAPEBASE_H

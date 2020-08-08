#include "Triangle.h"

#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include "src/shape/TriangleItem.h"
#include "src/property/PropObj.h"
#include <QDebug>

Triangle::Triangle(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(nullptr)
{

}

int Triangle::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new TriangleItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });
    return key;
}

void Triangle::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width*2, height*2);
}

void Triangle::CreateEnd(const QPointF &pos, GraphicsScene *pScene){
    m_pItem->Created();
}

void Triangle::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Triangle::SetEditable(bool editable){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setFlag(QGraphicsItem::ItemIsMovable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsFocusable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, editable);
    m_pItem->setAcceptHoverEvents(editable);
    m_pItem->setSelected(editable);
}

void Triangle::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Triangle::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Triangle::Rotate(qreal x, qreal y, qreal angle){
    QPointF pos = m_pItem->pos();
    qreal ang = trimAngle(angle);
    m_pItem->setTransformOriginPoint(m_pItem->sceneBoundingRect().center());
    m_pItem->setRotation(ang + m_rAngle);
    m_pItem->setPos(pos);
}

void Triangle::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

QRect Triangle::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Triangle::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Triangle::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF Triangle::GetPos(){
    return m_pItem->scenePos();
}

void Triangle::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *Triangle::GetGraphicsItem(){
    return m_pItem;
}

void Triangle::ChangeSize(qreal dx, qreal dy){
}

void Triangle::HideControls(bool hide){
    m_pItem->HideRotate(hide);
    m_pItem->HideClose(hide);
    m_pItem->HideResize(hide);
}

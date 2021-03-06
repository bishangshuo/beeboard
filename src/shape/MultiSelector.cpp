#include "MultiSelector.h"

#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include "src/shape/MultiSelectorItem.h"
#include "src/property/PropObj.h"
#include <QDebug>

MultiSelector::MultiSelector(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(nullptr)
{
    m_type = TOOL_TYPE::EMPTY;
}

MultiSelector::~MultiSelector(){
    if(m_pItem){
        delete m_pItem;
    }
}

int MultiSelector::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new MultiSelectorItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });

    m_pItem->SetRotateCallback([=](qreal anchor_x, qreal anchor_y, qreal _angle){
        emit sigRotate(anchor_x, anchor_y, _angle);
    });
    m_pItem->setZValue(INT_MAX);

    return key;
}

void MultiSelector::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width*2, height*2);
}

void MultiSelector::CreateEnd(GraphicsScene *pScene){
    m_pItem->Created();
}

void MultiSelector::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void MultiSelector::SetEditable(bool editable){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setFlag(QGraphicsItem::ItemIsMovable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsFocusable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, editable);
    m_pItem->setAcceptHoverEvents(editable);
    //m_pItem->setSelected(editable);
}

void MultiSelector::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void MultiSelector::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void MultiSelector::Rotate(qreal x, qreal y, qreal angle){
    QPointF pos = m_pItem->pos();
    qreal ang = trimAngle(angle);
    m_pItem->setTransformOriginPoint(m_pItem->sceneBoundingRect().center());
    m_pItem->setRotation(ang + m_rAngle);
    m_pItem->setPos(pos);
}

void MultiSelector::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

int MultiSelector::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect MultiSelector::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF MultiSelector::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF MultiSelector::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF MultiSelector::GetPos(){
    return m_pItem->scenePos();
}

QGraphicsItem *MultiSelector::GetGraphicsItem(){
    return m_pItem;
}

void MultiSelector::HideControls(bool hide){
    m_pItem->HideRotate(hide);
    m_pItem->HideClose(hide);
//    m_pItem->HideResize(hide);
}

QPen MultiSelector::GetPen() const{
    return m_pItem->GetPen();
}

QBrush MultiSelector::GetBrush() const{
    return m_pItem->GetBrush();
}

void MultiSelector::Undo(){

}

void MultiSelector::Redo(){

}

void MultiSelector::UndoEraser(int eraser) {

}

void MultiSelector::RedoEraser(int eraser) {

}

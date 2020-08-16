#include "Line.h"

#include <QGraphicsLineItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"
#include "src/shape/LineItem.h"
#include <QDebug>

Line::Line(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(NULL)
{
    m_type = TOOL_TYPE::LINE;
}

Line::~Line(){
    if(m_pItem){
        delete m_pItem;
    }
}

int Line::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new LineItem();
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    m_pItem->setLine(QLineF(leftTop, rightBottom));

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });
    m_pItem->SetItemChangedCallback([=](int _key){
        emit sigGeoChanged(reinterpret_cast<int>(m_pItem));
    });
    return key;
}

void Line::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setLine(QLineF(leftTop, rightBottom));
}

void Line::SetPos(const QPointF &pos){
    m_pItem->setPos(pos);
}

void Line::CreateEnd(GraphicsScene *pScene){
    m_pItem->Created();
    m_pItem->update();
}

void Line::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Line::SetEditable(bool editable){
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

void Line::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Line::RotateBegin(){
    m_rAngle = trimAngle(m_pItem->rotation());
}

void Line::Rotate(qreal x, qreal y, qreal angle){
    qDebug()<<"Line::Rotate angle="<<angle;
//    qreal ang = trimAngle(angle);
//    m_pItem->setTransformOriginPoint(m_pItem->boundingRect().center());
//    m_pItem->setRotation(ang + m_rAngle);
}

void Line::RotateEnd(){
    m_rAngle = trimAngle(m_pItem->rotation());
}

int Line::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect Line::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Line::GetP1(){
    return m_pItem->line().p1();
}

QPointF Line::GetP2(){
    return m_pItem->line().p2();
}

QPointF Line::GetPos(){
    return m_pItem->scenePos();
}

QGraphicsItem *Line::GetGraphicsItem(){
    return m_pItem;
}

void Line::HideControls(bool hide){
    m_pItem->HideClose(hide);
    m_pItem->HideResize(hide);
}

void Line::SetPen(QPen pen) {
    m_pItem->SetPen(pen);
}

void Line::SetBrush(QBrush brush) {
    m_pItem->SetBrush(brush);
}

QPen Line::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Line::GetBrush() const{
    return m_pItem->GetBrush();
}

void Line::Undo(){
    m_pItem->Undo();
}

void Line::Redo(){
    m_pItem->Redo();
}

void Line::UndoEraser(int eraser) {

}

void Line::RedoEraser(int eraser) {

}


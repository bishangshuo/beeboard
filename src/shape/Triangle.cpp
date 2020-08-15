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
    m_type = TOOL_TYPE::TRIANGLE;
}

Triangle::~Triangle(){
    if(m_pItem)
        delete m_pItem;
}

int Triangle::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new TriangleItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });

    connect(m_pItem, &ItemBase::sigItemChanged, [=](){
        emit sigGeoChanged(reinterpret_cast<int>(m_pItem));
    });
    return key;
}

void Triangle::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    m_pItem->setPos((leftTop+rightBottom)/2);
    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width, height);
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

int Triangle::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
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

qreal Triangle::GetAngle() const{
    return  m_pItem->rotation();
}

int Triangle::GetItemWidth() const{
    return m_pItem->Width();
}

int Triangle::GetItemHeight() const{
    return m_pItem->Height();
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

void Triangle::SetPen(QPen pen) {
    m_pItem->SetPen(pen);
}

void Triangle::SetBrush(QBrush brush) {
    m_pItem->SetBrush(brush);
}

QPen Triangle::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Triangle::GetBrush() const{
    return m_pItem->GetBrush();
}

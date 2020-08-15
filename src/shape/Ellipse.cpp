#include "Ellipse.h"

#include "src/shape/EllipseItem.h"
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

Ellipse::Ellipse(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(NULL)
{
    m_type = TOOL_TYPE::ELLIPSE;
}

Ellipse::~Ellipse(){
    if(m_pItem)
        delete m_pItem;
}

int Ellipse::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new EllipseItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
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

void Ellipse::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    m_pItem->setPos((leftTop+rightBottom)/2);
    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width, height);
}

void Ellipse::CreateEnd(GraphicsScene *pScene){
    m_pItem->Created();
}

void Ellipse::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Ellipse::SetEditable(bool editable){
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

void Ellipse::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Ellipse::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Ellipse::Rotate(qreal x, qreal y, qreal angle){
    m_pItem->Rotate(angle);
}

void Ellipse::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

int Ellipse::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect Ellipse::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Ellipse::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Ellipse::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF Ellipse::GetPos(){
    return m_pItem->scenePos();
}

qreal Ellipse::GetAngle() const{
    return  m_pItem->rotation();
}

int Ellipse::GetItemWidth() const{
    return m_pItem->Width();
}

int Ellipse::GetItemHeight() const{
    return m_pItem->Height();
}

void Ellipse::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *Ellipse::GetGraphicsItem(){
    return m_pItem;
}

void Ellipse::ChangeSize(qreal dx, qreal dy){

}

void Ellipse::HideControls(bool hide){
    m_pItem->HideRotate(hide);
    m_pItem->HideClose(hide);
    m_pItem->HideResize(hide);
}

void Ellipse::SetPen(QPen pen) {
    m_pItem->SetPen(pen);
}

void Ellipse::SetBrush(QBrush brush) {
    m_pItem->SetBrush(brush);
}

QPen Ellipse::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Ellipse::GetBrush() const{
    return m_pItem->GetBrush();
}

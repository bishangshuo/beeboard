#include "Rectangle.h"
#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include "src/shape/RectangleItem.h"
#include "src/property/PropObj.h"
#include <QDebug>

Rectangle::Rectangle(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(nullptr)
{
    m_type = TOOL_TYPE::RECTANGLE;
}

Rectangle::~Rectangle(){
    if(m_pItem)
        delete m_pItem;
}

int Rectangle::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new RectangleItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
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

void Rectangle::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    m_pItem->setPos((leftTop+rightBottom)/2);
    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width, height);
}

void Rectangle::CreateEnd(GraphicsScene *pScene){
    m_pItem->Created();
}

void Rectangle::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Rectangle::SetEditable(bool editable){
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

void Rectangle::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Rectangle::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Rectangle::Rotate(qreal x, qreal y, qreal angle){
    m_pItem->Rotate(angle);
}

void Rectangle::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

int Rectangle::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect Rectangle::GetRect(){
    //不要获取boundingrect，而是获取item内的rect
    return m_pItem->GetRect().toRect();
//    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Rectangle::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Rectangle::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF Rectangle::GetPos(){
    return m_pItem->scenePos();
}

qreal Rectangle::GetAngle() const{
    return  m_pItem->rotation();
}

int Rectangle::GetItemWidth() const{
    return m_pItem->Width();
}

int Rectangle::GetItemHeight() const{
    return m_pItem->Height();
}

void Rectangle::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *Rectangle::GetGraphicsItem(){
    return m_pItem;
}

void Rectangle::ChangeSize(qreal dx, qreal dy){
//    QRectF oldRect = m_pItem->rect();
//    QRectF newRect = QRectF(oldRect.x(), oldRect.y(), oldRect.width()+dx, oldRect.height()+dy);
//    //qDebug()<<"Rectangle::ChangeSize oldRect="<<oldRect<<", newRect="<<newRect;
//    if(newRect.width() < 10){
//        newRect.setWidth(10);
//    }
//    if(newRect.height() < 10){
//        newRect.setHeight(10);
//    }
//    m_pItem->setRect(newRect);
}

void Rectangle::HideControls(bool hide){
    m_pItem->HideRotate(hide);
    m_pItem->HideClose(hide);
    m_pItem->HideResize(hide);
}

void Rectangle::SetPen(QPen pen) {
    m_pItem->SetPen(pen);
}

void Rectangle::SetBrush(QBrush brush) {
    m_pItem->SetBrush(brush);
}

QPen Rectangle::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Rectangle::GetBrush() const{
    return m_pItem->GetBrush();
}

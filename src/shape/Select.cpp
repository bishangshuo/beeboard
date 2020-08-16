#include "Select.h"

#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include <QGraphicsRectItem>
#include "src/shape/SelectItem.h"
#include "src/property/PropObj.h"
#include <QDebug>

Select::Select(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(nullptr)
{
    m_type = TOOL_TYPE::SELECT;
}

Select::~Select(){
    if(m_pItem)
        delete m_pItem;
}

int Select::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new SelectItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    QPen pen;
    pen.setColor(QColor(255,0, 0, 128));
    pen.setWidth(1);
    pen.setStyle(Qt::DashDotLine);
    m_pItem->setPen(pen);
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    return key;
}

void Select::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    qreal x,y;
    x = leftTop.x() <= rightBottom.x() ? leftTop.x() : rightBottom.x();
    y = leftTop.y() <= rightBottom.y() ? leftTop.y() : rightBottom.y();
    qreal width = abs(rightBottom.x() - leftTop.x());
    qreal height = abs(rightBottom.y() - leftTop.y());

    QRectF rc = QRectF(x, y, width,  height);
    //qDebug() << "Select::UpdateRect " << rc;
    m_pItem->setRect(rc);
}

void Select::CreateEnd(GraphicsScene *pScene){
    //m_pItem->Created();
}

void Select::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Select::SetEditable(bool editable){
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

void Select::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Select::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Select::Rotate(qreal x, qreal y, qreal angle){
    QPointF pos = m_pItem->pos();
    qreal ang = trimAngle(angle);
    m_pItem->setTransformOriginPoint(m_pItem->sceneBoundingRect().center());
    m_pItem->setRotation(ang + m_rAngle);
    m_pItem->setPos(pos);
}

void Select::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

int Select::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect Select::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Select::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Select::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF Select::GetPos(){
    return m_pItem->scenePos();
}

QGraphicsItem *Select::GetGraphicsItem(){
    return m_pItem;
}

void Select::HideControls(bool hide){
    //m_pItem->HideBaseControls(hide);
}

QPen Select::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Select::GetBrush() const{
    return m_pItem->GetBrush();
}

void Select::Undo(){

}

void Select::Redo(){

}

void Select::UndoEraser(int eraser) {

}

void Select::RedoEraser(int eraser) {

}

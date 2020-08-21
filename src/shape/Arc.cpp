#include "Arc.h"
#include "src/shape/ArcItem.h"
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

Arc::Arc(QObject *parent)
     : ShapeBase(parent)
     , m_pItem(NULL)
 {
     m_type = TOOL_TYPE::ARC;
 }

Arc::~Arc(){

}

int Arc::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new ArcItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(reinterpret_cast<int>(m_pItem));
    });

    m_pItem->SetItemChangedCallback([=](int _key){
        emit sigGeoChanged(reinterpret_cast<int>(m_pItem));
    });

    return key;
}

void Arc::SetStartAngle(int startAngle){
    int rstartangle = startAngle*16;
    m_pItem->setStartAngle(rstartangle);
    m_pItem->setSpanAngle(0);
}

void Arc::SetSpanAngle(int spanAgnle){
    int rangle = spanAgnle*16;
    m_pItem->setSpanAngle(rangle);
    QPointF pos = m_pItem->boundingRect().center();
    m_pItem->setPos(pos);
}

void Arc::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    m_pItem->setPos((leftTop+rightBottom)/2);
    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->setRect(leftTop.x(), leftTop.y(), width, height);
}

void Arc::CreateEnd(GraphicsScene *pScene){
    m_pItem->Created();
}

void Arc::SetSelected(bool selected){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void Arc::SetEditable(bool editable){
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

void Arc::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Arc::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Arc::Rotate(qreal x, qreal y, qreal angle){
    m_pItem->Rotate(angle);
}

void Arc::RotateEnd(){
    m_rAngle = m_pItem->rotation();
}

int Arc::GetItemKey() const{
    return reinterpret_cast<int>(m_pItem);
}

QRect Arc::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Arc::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Arc::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

QPointF Arc::GetPos(){
    return m_pItem->scenePos();
}

qreal Arc::GetAngle() const{
    return  m_pItem->rotation();
}

int Arc::GetItemWidth() const{
    return m_pItem->Width();
}

int Arc::GetItemHeight() const{
    return m_pItem->Height();
}

QGraphicsItem *Arc::GetGraphicsItem(){
    return m_pItem;
}

void Arc::HideControls(bool hide){
    m_pItem->HideRotate(hide);
    m_pItem->HideClose(hide);
}

void Arc::SetPen(QPen pen) {
    m_pItem->SetPen(pen);
}

void Arc::SetBrush(QBrush brush) {
    m_pItem->SetBrush(brush);
}

QPen Arc::GetPen() const{
    return m_pItem->GetPen();
}

QBrush Arc::GetBrush() const{
    return m_pItem->GetBrush();
}

void Arc::Undo(){
    m_pItem->Undo();
}

void Arc::Redo(){
    m_pItem->Redo();
}

void Arc::UndoEraser(int eraser) {

}

void Arc::RedoEraser(int eraser) {

}

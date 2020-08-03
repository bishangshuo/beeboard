#include "Ellipse.h"

#include "src/shape/EllipseItem.h"
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

Ellipse::Ellipse(QObject *parent)
    : ShapeBase(parent)
{

}

int Ellipse::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new EllipseItem(leftTop.x(), leftTop.y(), rightBottom.x()-leftTop.x(), rightBottom.y()-leftTop.y());
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });
    return key;
}

void Ellipse::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    int width = abs(rightBottom.x()-leftTop.x());
    int height = abs(rightBottom.y()-leftTop.y());
    m_pItem->UpdateSize(width*2, height*2);
}

void Ellipse::CreateEnd(){
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

void Ellipse::Rotate(qreal angle){
}

void Ellipse::RotateEnd(){
    m_rAngle = m_pItem->rotation();
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

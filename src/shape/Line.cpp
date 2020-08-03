#include "Line.h"

#include <QGraphicsLineItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"
#include "src/shape/LineItem.h"
#include <QDebug>

Line::Line(QObject *parent)
    : ShapeBase(parent)
{

}

int Line::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new LineItem();
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    m_pItem->setPen(pen);
    m_pItem->setLine(QLineF(leftTop, rightBottom));

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });
    return key;
}

void Line::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    m_pItem->setLine(QLineF(leftTop, rightBottom));
}

void Line::CreateEnd(){

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

void Line::Rotate(qreal angle){
    qDebug()<<"Line::Rotate angle="<<angle;
//    qreal ang = trimAngle(angle);
//    m_pItem->setTransformOriginPoint(m_pItem->boundingRect().center());
//    m_pItem->setRotation(ang + m_rAngle);
}

void Line::RotateEnd(){
    m_rAngle = trimAngle(m_pItem->rotation());
}

QRect Line::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Line::GetP1(){
    return QPointF();//m_pItem->line().p1();
}

QPointF Line::GetP2(){
    return QPointF();//m_pItem->line().p2();
}

QPointF Line::GetPos(){
    return m_pItem->scenePos();
}

void Line::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *Line::GetGraphicsItem(){
    return m_pItem;
}

void Line::ChangeSize(qreal dx, qreal dy){
//    QPointF p1 = m_pItem->line().p1();
//    QPointF p2 = m_pItem->line().p2();
//    QPointF p3 = QPointF(p2.x()+dx, p2.y()+dy);
//    m_pItem->setLine(p1.x(), p1.y(), p3.x(), p3.y());
}

void Line::HideControls(bool hide){
    m_pItem->HideClose(hide);
    m_pItem->HideResize(hide);
}

#include "Line.h"

#include <QGraphicsLineItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"
#include "src/shape/GraphicsLineItem.h"
#include <QDebug>

Line::Line(QObject *parent)
    : ShapeBase(parent)
{

}

int Line::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new GraphicsLineItem();
    pScene->addItem(m_pItem);
    m_pItem->setPen(QPen(PropObj::GetInstance()->PenColor(), PropObj::GetInstance()->PenWidth()));
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setLine(leftTop.x(), leftTop.y(), rightBottom.x(), rightBottom.y());
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    //m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    return key;
}

void Line::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setLine(leftTop.x(), leftTop.y(), rightBottom.x(), rightBottom.y());
    m_pItem->update();
}

void Line::SetSelected(){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(true);
}

void Line::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Line::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Line::Rotate(qreal angle){
    qreal ang = trimAngle(angle);
    m_pItem->setTransformOriginPoint(m_pItem->boundingRect().center());
    m_pItem->setRotation(ang + m_rAngle);
}

void Line::RotateEnd(){
    m_rAngle = m_pItem->rotation();
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

void Line::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *Line::GetGraphicsItem(){
    return m_pItem;
}

void Line::ChangeSize(qreal dx, qreal dy){
    QPointF p1 = m_pItem->line().p1();
    QPointF p2 = m_pItem->line().p2();
    QPointF p3 = QPointF(p2.x()+dx, p2.y()+dy);
    m_pItem->setLine(p1.x(), p1.y(), p3.x(), p3.y());
}

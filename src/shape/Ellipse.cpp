#include "Ellipse.h"

#include <QGraphicsEllipseItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

Ellipse::Ellipse(QObject *parent)
    : ShapeBase(parent)
{

}

int Ellipse::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new QGraphicsEllipseItem();
    pScene->addItem(m_pItem);
    m_pItem->setPen(QPen(PropObj::GetInstance()->PenColor(), PropObj::GetInstance()->PenWidth()));
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setRect(QRectF(leftTop, rightBottom));
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    //m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    return key;
}

void Ellipse::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }

    QPointF p1 = leftTop;
    QPointF p2 = rightBottom;
    swapPointIf(p1, p2);

    m_pItem->setRect(QRectF(p1, p2));
    m_pItem->update();
}

void Ellipse::SetSelected(){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(true);
}

void Ellipse::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

void Ellipse::RotateBegin(){
    m_rAngle = m_pItem->rotation();
}

void Ellipse::Rotate(qreal angle){
    qreal ang = trimAngle(angle);
    m_pItem->setTransformOriginPoint(m_pItem->boundingRect().center());
    m_pItem->setRotation(ang + m_rAngle);
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
    QRectF oldRect = m_pItem->rect();
    QRectF newRect = QRectF(oldRect.x(), oldRect.y(), oldRect.width()+dx, oldRect.height()+dy);
    if(newRect.width() < 10){
        newRect.setWidth(10);
    }
    if(newRect.height() < 10){
        newRect.setHeight(10);
    }
    m_pItem->setRect(newRect);
}

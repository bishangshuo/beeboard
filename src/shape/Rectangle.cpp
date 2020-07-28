#include "Rectangle.h"
#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"
#include <QtDebug>

Rectangle::Rectangle(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(nullptr)
{

}

int Rectangle::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new QGraphicsRectItem();
    pScene->addItem(m_pItem);
    m_pItem->setPen(QPen(PropObj::GetInstance()->PenColor(), PropObj::GetInstance()->PenWidth()));
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setRect(QRectF(leftTop, rightBottom));
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);
    m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    return key;
}

void Rectangle::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    if(m_pItem == nullptr){
        return;
    }
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setRect(QRectF(leftTop, rightBottom));
    m_pItem->update();
}

void Rectangle::SetSelected(){
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(true);
}

void Rectangle::Remove(GraphicsScene *pScene){
   pScene->removeItem(m_pItem);
}

QRect Rectangle::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Rectangle::GetP1(){
    return m_pItem->sceneBoundingRect().topLeft();
}

QPointF Rectangle::GetP2(){
    return m_pItem->sceneBoundingRect().bottomRight();
}

void Rectangle::ChangePos(qreal dx, qreal dy){
    QPointF pos = m_pItem->scenePos();
    QPointF newPos = pos + QPointF(dx, dy);
    m_pItem->setPos(newPos);
}

QGraphicsItem *Rectangle::GetGraphicsItem(){
    return m_pItem;
}

void Rectangle::ChangeSize(qreal dx, qreal dy){
    QRectF oldRect = m_pItem->rect();
    QRectF newRect = QRectF(oldRect.x(), oldRect.y(), oldRect.width()+dx, oldRect.height()+dy);
    //qDebug()<<"Rectangle::ChangeSize oldRect="<<oldRect<<", newRect="<<newRect;
    if(newRect.width() < 10){
        newRect.setWidth(10);
    }
    if(newRect.height() < 10){
        newRect.setHeight(10);
    }
    m_pItem->setRect(newRect);
}

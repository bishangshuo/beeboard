#include "Rectangle.h"
#include <QGraphicsRectItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

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
    //m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
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

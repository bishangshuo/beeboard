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
    QPointF pos = (leftTop + rightBottom) / 2;
    m_pItem->setRect(QRectF(leftTop, rightBottom));
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

QRect Ellipse::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

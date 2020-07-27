#include "Line.h"

#include <QGraphicsLineItem>
#include "src/graphics/GraphicsScene.h"
#include "src/property/PropObj.h"

Line::Line(QObject *parent)
    : ShapeBase(parent)
{

}

int Line::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    m_pItem = new QGraphicsLineItem();
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

QRect Line::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

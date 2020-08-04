#include "Pencil.h"
#include "src/shape/PencilItem.h"
#include "src/graphics/GraphicsScene.h"
#include <QPainter>
#include <QGraphicsPathItem>
#include <QDebug>

inline QPixmap RenderPath(const QPainterPath &path){
    QRectF rc = path.boundingRect();
    QPixmap pixmap(rc.width(), rc.height());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPath(path);
    painter.end();
    return pixmap;
}

Pencil::Pencil(QObject *parent)
    : ShapeBase(parent)
{

}

int Pencil::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) {
    m_pItem = new PencilItem();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    m_pItem->setPen(pen);
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });

    m_path.moveTo(leftTop);
    m_pItem->setPath(m_path);
    return key;
}
void Pencil::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) {
    m_path.lineTo(rightBottom);

    m_pItem->setPath(m_path);
}

void Pencil::CreateEnd(GraphicsScene *pScene) {
    m_pItem->Created();
}
void Pencil::SetSelected(bool selected) {
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}
void Pencil::SetEditable(bool editable) {

}
void Pencil::Remove(GraphicsScene *pScene) {
    pScene->removeItem(m_pItem);
}

void Pencil::RotateBegin() {

}
void Pencil::Rotate(qreal x, qreal y, qreal angle) {

}
void Pencil::RotateEnd() {

}

QRect Pencil::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Pencil::GetP1() {
    return QPointF();
}
QPointF Pencil::GetP2() {
    return QPointF();
}
QPointF Pencil::GetPos() {
    return QPointF();
}
void Pencil::ChangePos(qreal dx, qreal dy) {

}
QGraphicsItem *Pencil::GetGraphicsItem() {
    return m_pItem;
}
void Pencil::ChangeSize(qreal dx, qreal dy) {

}

void Pencil::HideControls(bool hide) {

}

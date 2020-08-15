#include "Eraser.h"

Eraser::Eraser(QObject *parent)
    : ShapeBase(parent)
{
    m_type = TOOL_TYPE::ERASER;
}

Eraser::~Eraser(){

}

int Eraser::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    return 0;
}

void Eraser::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
}

void Eraser::CreateEnd(GraphicsScene *pScene){
}

void Eraser::SetSelected(bool selected){
}

void Eraser::SetEditable(bool editable){
}

void Eraser::Remove(GraphicsScene *pScene){
}

void Eraser::RotateBegin(){
}

void Eraser::Rotate(qreal x, qreal y, qreal angle){
}

void Eraser::RotateEnd(){
}

int Eraser::GetItemKey() const{
    return 0;
}

QRect Eraser::GetRect(){
    return QRect();
}

QPointF Eraser::GetP1(){
    return QPointF();
}

QPointF Eraser::GetP2(){
    return QPointF();
}

QPointF Eraser::GetPos(){
    return QPointF();
}

void Eraser::ChangePos(qreal dx, qreal dy){
}

QGraphicsItem *Eraser::GetGraphicsItem(){
    return NULL;
}

void Eraser::ChangeSize(qreal dx, qreal dy){

}

void Eraser::HideControls(bool hide){
}

QPen Eraser::GetPen() const{
    return QPen();
}

QBrush Eraser::GetBrush() const{
    return QBrush();
}

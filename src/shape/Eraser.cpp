#include "Eraser.h"

Eraser::Eraser(QObject *parent)
    : ShapeBase(parent)
{

}

int Eraser::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
    return 0;
}

void Eraser::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene){
}

void Eraser::CreateEnd(const QPointF &pos, GraphicsScene *pScene){
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

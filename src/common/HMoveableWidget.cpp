#include "HMoveableWidget.h"

#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>

HMoveableWidget::HMoveableWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    border_px = 4;
    posBy = pos_none;
    m_bDisabledMove = false;
    m_bDesabledResize = false;
    setMouseTracking(true);
}


void HMoveableWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)
    {
        win_pos = event->globalPos() - frameGeometry().topLeft();
        m_ptMoveBegin = event->globalPos() - win_pos;
    }
}

void HMoveableWidget::mouseMoveEvent(QMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton && !m_bDisabledMove){

        if(posBy == pos_none){

            QPoint movePoint = event->globalPos() - win_pos;
            move(movePoint);
            QRect rectChanged = QRect(movePoint,
                                      QPoint(movePoint.x()+rect().width(),
                                             movePoint.y()+rect().height()));
            //OnRectChanged(rectChanged);
            int dx = movePoint.x()-m_ptMoveBegin.x();
            int dy = movePoint.y()-m_ptMoveBegin.y();
            OnPosChanged(dx, dy);
            m_ptMoveBegin = movePoint;
        }
        else{
            if(m_bDesabledResize)
                return;
            int m_top,m_bottom,m_left,pright;
            m_top = frameGeometry().top();
            m_bottom =frameGeometry().bottom();
            m_left = frameGeometry().left();
            pright = frameGeometry().right();
            if(posBy & top){
                if(height() == minimumHeight()){
                    m_top = qMin(event->y()+this->pos().y(),m_top);
                }
                else if(height() == maximumHeight()){
                    m_top = qMax(event->y()+this->pos().y(),m_top);
                }
                else{
                    m_top = event->y()+this->pos().y();
                }
            }
            else if(posBy & bottom){
                if(height() == minimumHeight()){
                    m_bottom = qMax(event->y()+this->pos().y(),m_top);
                }
                else if(height() == maximumHeight()){
                    m_bottom = qMin(event->y()+this->pos().y(),m_top);
                }
                else{
                    m_bottom = event->y()+this->pos().y();
                }
            }

            if(posBy & left){
                if(width() == minimumWidth()){
                    m_left = qMin(event->x()+this->pos().x(),m_left);
                }
                else if(width() == maximumWidth()){
                    m_left = qMax(event->x()+this->pos().x(),m_left);
                }
                else{
                    m_left = event->x()+this->pos().x();
                }
            }
            else if(posBy & right){
                if(width() == minimumWidth()){
                    pright = qMax(event->x()+this->pos().x(),pright);
                }
                else if(width() == maximumWidth()){
                    pright = qMin(event->x()+this->pos().x(),pright);
                }
                else{
                    pright =event->x()+this->pos().x();
                }
            }

            QRect rectChanged = QRect(QPoint(m_left,m_top),QPoint(pright,m_bottom));
            setGeometry(rectChanged);
            OnRectChanged(rectChanged);
        }
    }
    else arrows_update();


}
void HMoveableWidget::arrows_update()
{
    if(m_bDesabledResize)
        return;
    int diffLeft = abs(mapFromGlobal(cursor().pos()).x() -frameGeometry().left()+this->pos().x());
    int diffRight = abs(mapFromGlobal(cursor().pos()).x() -frameGeometry().right()+this->pos().x());
    int diffTop = abs(mapFromGlobal(cursor().pos()).y() - frameGeometry().top()+this->pos().y());
    int diffBottom = abs(mapFromGlobal(cursor().pos()).y() - frameGeometry().bottom()+this->pos().y());


    QCursor my_cursor;
    my_cursor = cursor();
    if(diffTop < border_px){
        if(diffLeft < border_px){
            posBy = top_Left;
            my_cursor.setShape(Qt::SizeFDiagCursor);
        }
        else if(diffRight < border_px){
            posBy = topRight;
            my_cursor.setShape(Qt::SizeBDiagCursor);
        }
        else{
            posBy = top;
            my_cursor.setShape(Qt::SizeVerCursor);
        }
    }
    else if(diffBottom < border_px){
        if(diffLeft < border_px){
            posBy = bottomLeft;
            my_cursor.setShape(Qt::SizeBDiagCursor);
        }
        else if(diffRight < border_px){
            posBy = bottomRight;
            my_cursor.setShape(Qt::SizeFDiagCursor);
        }
        else{
            posBy = bottom;
            my_cursor.setShape(Qt::SizeVerCursor);
        }
    }
    else if(diffLeft < border_px){
        posBy = left;
        my_cursor.setShape(Qt::SizeHorCursor);
    }
    else if(diffRight < border_px){
        posBy = right;
        my_cursor.setShape(Qt::SizeHorCursor);
    }
    else{
        posBy = pos_none;
        my_cursor.setShape(Qt::ArrowCursor);
    }

    setCursor(my_cursor);
}
void HMoveableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    if(posBy != pos_none){
        arrows_update();
        OnRectChangeCompleted();
    }else{
        arrows_update();
        OnPosChangeCompleted();
    }
}

void HMoveableWidget::OnPosChanged(int dx, int dy){

}

void HMoveableWidget::OnPosChangeCompleted(){

}

void HMoveableWidget::OnRectChanged(const QRect &rect)
{

}

void HMoveableWidget::OnRectChangeCompleted(){

}


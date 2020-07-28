#include "TrackButton.h"
#include <QMouseEvent>

TrackButton::TrackButton(QWidget *parent)
    : QPushButton(parent), m_bTracking(false){

}

TrackButton::TrackButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent){

}

TrackButton::TrackButton(const QIcon& icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent){

}

void TrackButton::mousePressEvent(QMouseEvent *e) {
    //不要把消息发送到基类，否则基类将转发消息给父窗口，干扰父窗口的业务逻辑
    //QPushButton::mousePressEvent(e);
    m_bTracking = true;
    QPoint pos = e->pos();
    pos = mapToParent(pos);
    emit sigMousePressed(pos);
}

void TrackButton::mouseMoveEvent(QMouseEvent *e) {
    //不要把消息发送到基类，否则基类将转发消息给父窗口，干扰父窗口的业务逻辑
    //QPushButton::mouseMoveEvent(e);
    if(m_bTracking){
        QPoint pos = e->pos();
        pos = mapToParent(pos);
        emit sigMouseMove(pos);
    }
}

void TrackButton::mouseReleaseEvent(QMouseEvent *e) {
    //不要把消息发送到基类，否则基类将转发消息给父窗口，干扰父窗口的业务逻辑
    //QPushButton::mouseReleaseEvent(e);
    m_bTracking = false;
    QPoint pos = e->pos();
    pos = mapToParent(pos);
    emit sigMouseReleased(pos);
}

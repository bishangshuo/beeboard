#ifndef HMOVEABLEWIDGET_H
#define HMOVEABLEWIDGET_H

#include <QWidget>

class HMoveableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HMoveableWidget(QWidget *parent = nullptr);

    void DisabledMove(bool disabled){
        m_bDisabledMove = disabled;
    }

    void DisabledResize(bool disabled){
        m_bDesabledResize = disabled;
    }
public:
    virtual void OnPosChanged(int dx, int dy);
    virtual void OnPosChangeCompleted();

    virtual void OnRectChanged(const QRect &rect);
    virtual void OnRectChangeCompleted();
public slots:

protected:


    void arrows_update();
    QPoint win_pos;
    QPoint m_ptMoveBegin;
    int    border_px;
    enum {pos_none,
              top = 0x01,
              bottom = 0x02,
              left = 0x04,
              right = 0x08,
              top_Left = 0x01 | 0x04,
              topRight = 0x01 | 0x08,
              bottomLeft = 0x02 | 0x04,
              bottomRight = 0x02 | 0x08} posBy;


    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);

    bool m_bDisabledMove;
    bool m_bDesabledResize;
};

#endif // HMOVEABLEWIDGET_H

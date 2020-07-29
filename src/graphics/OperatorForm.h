#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>

#include "src/common/HMoveableWidget.h"
#include "src/common/ToolType.h"

class QPushButton;
class TrackButton;

namespace Ui {
class OperatorForm;
}

class OperatorForm : public HMoveableWidget
{
    Q_OBJECT

public:
    explicit OperatorForm(int key, TOOL_TYPE::Type toolType, QWidget *parent = nullptr);
    ~OperatorForm();
    void setKey(int key){
        m_nKey = key;
    }

    static int EDGE_WIDTH(){
        return  30;
    }
    void showControls();
    void setToolType(TOOL_TYPE::Type toolType){
        m_eToolType = toolType;
    }
    void setPoints(const QPoint &p1, const QPoint &p2);
signals:
    void sigPosChanged(int key, int dx, int dy);
    void sigItemRemove(int key);
    void sigResizeItemBegin(int key);
    void sigResizeItem(int key, int dx, int dy);
    void sigResizeItemEnd(int key);

    void sigRotateItemBegin(int key);
    void sigRotateItem(int key, qreal angle);
    void sigRotateItemEnd(int key);
public slots:
    void slotResizeBtnPressed(const QPoint &pos);
    void slotResizeBtnMove(const QPoint &pos);
    void slotResizeBtnReleased(const QPoint &pos);

    void slotRotateBtnPressed(const QPoint &pos);
    void slotRotateBtnMove(const QPoint &pos);
    void slotRotateBtnReleased(const QPoint &pos);
protected:
    void OnPosChanged(int dx, int dy) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    void setupButtons();
    void posButtons();
private:
    Ui::OperatorForm *ui;
    int m_nKey;
    TOOL_TYPE::Type m_eToolType;
    QPushButton *m_pBtnRemove;
    TrackButton *m_pBtnRotate;
    TrackButton *m_pBtnResize;
    TrackButton *m_pBtnLinePoint;

    bool m_bDraging;
    QPoint m_ptResizeBegin;
    QPoint m_ptP1, m_ptP2;

    bool m_bRotating;
    QPoint p0,p1,p2;
};

#endif // OPERATORFORM_H

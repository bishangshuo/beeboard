#include "OperatorForm.h"
#include "ui_OperatorForm.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "src/common/TrackButton.h"

OperatorForm::OperatorForm(int key, TOOL_TYPE::Type toolType, QWidget *parent)
    : HMoveableWidget(parent)
    , ui(new Ui::OperatorForm)
    , m_nKey(key)
    , m_eToolType(toolType)
    , m_bDraging(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground,true);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(1);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(0, 0, 0, 0));
    setAutoFillBackground(true);
    setPalette(pal);

    setupButtons();
}

OperatorForm::~OperatorForm()
{
    delete ui;
}

void OperatorForm::OnPosChanged(int dx, int dy){
//    qDebug() << "OperatorForm::OnPosChanged dx = " << dx << ", dy = " << dy;
    emit sigPosChanged(m_nKey, dx, dy);
}

void OperatorForm::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
}

void OperatorForm::setupButtons(){
    QVBoxLayout *mainLy = new QVBoxLayout();
    setLayout(mainLy);
    mainLy->setMargin(0);

    QHBoxLayout *topLy = new QHBoxLayout();
    mainLy->addLayout(topLy);
    topLy->setMargin(0);

    m_pBtnRotate = new QPushButton();
    topLy->addWidget(m_pBtnRotate);
    topLy->setAlignment(Qt::AlignCenter|Qt::AlignTop);
    QIcon icon;
    icon.addFile(tr(":/resources/images/rotate-left.png"));
    m_pBtnRotate->setIcon(icon);
    m_pBtnRotate->setIconSize(QSize(24,24));
    m_pBtnRotate->setStyleSheet("background-color:rgba(0,0,0,0)");
    m_pBtnRotate->setFocusPolicy(Qt::NoFocus);

    connect(m_pBtnRotate, &QPushButton::clicked, [=](){});

    QHBoxLayout *bottomLy = new QHBoxLayout();
    mainLy->addLayout(bottomLy);
    bottomLy->setMargin(0);

    m_pBtnRemove = new QPushButton();
    bottomLy->addWidget(m_pBtnRemove);
    bottomLy->setAlignment(m_pBtnRemove, Qt::AlignLeft);
    QIcon icon2;
    icon2.addFile(tr(":/resources/images/delete.png"));
    m_pBtnRemove->setIcon(icon2);
    m_pBtnRemove->setIconSize(QSize(24,24));
    m_pBtnRemove->setStyleSheet("background-color:rgba(0,0,0,0)");
    m_pBtnRemove->setFocusPolicy(Qt::NoFocus);

    connect(m_pBtnRemove, &QPushButton::clicked, [=](){
        emit sigItemRemove(m_nKey);
    });

    m_pBtnResize = new TrackButton(this);
    m_pBtnResize->setMouseTracking(true);
//    bottomLy->addWidget(m_pBtnResize);
//    bottomLy->setAlignment(m_pBtnResize, Qt::AlignRight);
    QIcon icon3;
    icon3.addFile(tr(":/resources/images/zoom.png"));
    m_pBtnResize->setIcon(icon3);
    m_pBtnResize->setIconSize(QSize(24,24));
    m_pBtnResize->setStyleSheet("background-color:rgba(0,0,0,0)");
    m_pBtnResize->setFocusPolicy(Qt::NoFocus);
    m_pBtnResize->setCursor(Qt::SizeAllCursor);

    connect(m_pBtnResize, SIGNAL(sigMousePressed(const QPoint &)), this, SLOT(slotResizeBtnPressed(const QPoint &)));
    connect(m_pBtnResize, SIGNAL(sigMouseMove(const QPoint &)), this, SLOT(slotResizeBtnMove(const QPoint &)));
    connect(m_pBtnResize, SIGNAL(sigMouseReleased(const QPoint &)), this, SLOT(slotResizeBtnReleased(const QPoint &)));

    m_pBtnLinePoint = new TrackButton(this);
    m_pBtnLinePoint->setMouseTracking(true);
    //bottomLy->addWidget(m_pBtnLinePoint);
    //bottomLy->setAlignment(m_pBtnLinePoint, Qt::AlignRight);
    QIcon icon4;
    icon4.addFile(tr(":/resources/images/repos-line.png"));
    m_pBtnLinePoint->setIcon(icon4);
    m_pBtnLinePoint->setIconSize(QSize(24,24));
    m_pBtnLinePoint->setStyleSheet("background-color:rgba(0,0,0,0)");
    m_pBtnLinePoint->setFocusPolicy(Qt::NoFocus);
    m_pBtnLinePoint->setCursor(Qt::SizeAllCursor);

    connect(m_pBtnLinePoint, SIGNAL(sigMousePressed(const QPoint &)), this, SLOT(slotResizeBtnPressed(const QPoint &)));
    connect(m_pBtnLinePoint, SIGNAL(sigMouseMove(const QPoint &)), this, SLOT(slotResizeBtnMove(const QPoint &)));
    connect(m_pBtnLinePoint, SIGNAL(sigMouseReleased(const QPoint &)), this, SLOT(slotResizeBtnReleased(const QPoint &)));

    showControls();
}

void OperatorForm::posButtons(){

}

void OperatorForm::slotResizeBtnPressed(const QPoint &pos) {
    m_bDraging = true;
    m_ptResizeBegin = pos;
    m_pBtnRemove->hide();
    m_pBtnRotate->hide();
    m_pBtnLinePoint->setIcon(QIcon(""));
    emit sigResizeItemBegin(m_nKey);
}

void OperatorForm::slotResizeBtnMove(const QPoint &pos) {
    int dx = pos.x() - m_ptResizeBegin.x();
    int dy = pos.y() - m_ptResizeBegin.y();
    m_ptResizeBegin = pos;

    emit sigResizeItem(m_nKey, dx, dy);
}

void OperatorForm::slotResizeBtnReleased(const QPoint &pos) {
    Q_UNUSED(pos)
    emit sigResizeItemEnd(m_nKey);
    m_bDraging = false;
    m_pBtnLinePoint->setIcon(QIcon(":/resources/images/repos-line.png"));
    showControls();
    m_pBtnRemove->show();
    m_pBtnRotate->show();
}

void OperatorForm::showControls(){
    if(TOOL_TYPE::LINE == m_eToolType){
        m_pBtnResize->hide();
        //qDebug() << "OperatorForm::showControls m_ptP1="<<m_ptP1<<", m_ptP2=" <<m_ptP2;
        m_pBtnLinePoint->setGeometry(m_ptP2.x()-12, m_ptP2.y()-12, 24, 24);
        m_pBtnLinePoint->show();
        m_pBtnLinePoint->raise();
    }else{
        m_pBtnLinePoint->hide();
        QSize sz = size();
        m_pBtnResize->setGeometry(sz.width()-24, sz.height()-24, 24, 24);
        m_pBtnResize->show();
        m_pBtnResize->raise();
    }
}

void OperatorForm::setPoints(const QPoint &p1, const QPoint &p2){
    m_ptP1 = p1;
    m_ptP2 = p2;
    showControls();
}

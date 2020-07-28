#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "src/graphics/GraphicsScene.h"
#include "src/graphics/GraphicsView.h"
#include "src/graphics/OperatorForm.h"
#include "src/common/ToolType.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_eToolType(TOOL_TYPE::SELECT)
    , m_pScene(nullptr)
    , m_pView(nullptr)
    , m_pOperatorForm(nullptr)
{
    ui->setupUi(this);
    //showFullScreen();
    //resize(1280, 768);
    setupActions();
    initGraphics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event){

    if(m_pScene != nullptr && m_pView != nullptr){
        QSizeF sizef = size();
        QSize tsize = ui->toolbar->size();

        qreal sceneWidth = sizef.width()*2;
        qreal sceneHeight = sizef.height()*2-tsize.height();

        m_pScene->setSceneRect(-sceneWidth/2, sceneHeight/2, sceneWidth, sceneHeight);
        m_pView->setGeometry(0, tsize.height(), sizef.width(), sizef.height()-tsize.height());
    }
}

void MainWindow::setupActions(){
    ui->actionSelect->setData(QVariant(TOOL_TYPE::SELECT));
    ui->actionPencil->setData(QVariant(TOOL_TYPE::PENCIL));
    ui->actionRectangle->setData(QVariant(TOOL_TYPE::RECTANGLE));
    ui->actionEllipse->setData(QVariant(TOOL_TYPE::ELLIPSE));
    ui->actionLine->setData(QVariant(TOOL_TYPE::LINE));
    ui->actionEraser->setData(QVariant(TOOL_TYPE::ERASER));

    ui->actionUndo->setData(QVariant(TOOL_TYPE::UNDO));
    ui->actionRedo->setData(QVariant(TOOL_TYPE::REDO));

    ui->actionMove->setData(QVariant(TOOL_TYPE::MOVE));

    ui->actionZoomin->setData(QVariant(TOOL_TYPE::ZOOMIN));
    ui->actionZoomout->setData(QVariant(TOOL_TYPE::ZOOMOUT));

    ui->actionClear->setData(QVariant(TOOL_TYPE::CLEAR));

    m_pActionGroup = new QActionGroup(this);
    m_pActionGroup->addAction(ui->actionSelect);
    m_pActionGroup->addAction(ui->actionPencil);
    m_pActionGroup->addAction(ui->actionRectangle);
    m_pActionGroup->addAction(ui->actionEllipse);
    m_pActionGroup->addAction(ui->actionLine);
    m_pActionGroup->addAction(ui->actionEraser);
    m_pActionGroup->addAction(ui->actionMove);
    m_pActionGroup->addAction(ui->actionZoomin);
    m_pActionGroup->addAction(ui->actionZoomout);
    connect(m_pActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotActionGroup(QAction *)));

    connect(ui->actionUndo, &QAction::triggered, [=](){
        qDebug() << "on undo";
        hideOperatorForm();
        m_pView->SetToolType(TOOL_TYPE::UNDO);
    });
    connect(ui->actionRedo, &QAction::triggered, [=](){
        qDebug() << "on redo";
        hideOperatorForm();
        m_pView->SetToolType(TOOL_TYPE::REDO);
    });

    connect(ui->actionMove, &QAction::triggered, [=](){
        qDebug() << "on move";
        hideOperatorForm();
        m_pView->SetToolType(TOOL_TYPE::MOVE);
        m_pView->beginMove();
    });

    connect(ui->actionReset, &QAction::triggered, [=](){
        qDebug() << "on reset";
        hideOperatorForm();
        m_pView->SetToolType(TOOL_TYPE::RESET);
        m_pView->reset();
    });

    connect(ui->actionClear, &QAction::triggered, [=](){
        qDebug() << "on clear";
        hideOperatorForm();
        m_pView->SetToolType(TOOL_TYPE::CLEAR);
        m_pScene->clearScene();
    });

    ui->actionSelect->setChecked(true);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
}

void MainWindow::initGraphics(){
    QSizeF sizef = size();
    QSize tsize = ui->toolbar->size();

    qreal sceneWidth = sizef.width()*2;
    qreal sceneHeight = sizef.height()*2-tsize.height();
    m_pScene = new GraphicsScene(this);
    m_pScene->setSceneRect(-sceneWidth/2, -sceneHeight/2, sceneWidth, sceneHeight);

    m_pView = new GraphicsView(this);
    m_pView->setScene(m_pScene);
    m_pView->setGeometry(0, tsize.height(), sizef.width(), sizef.height()-tsize.height());
    m_pView->setRenderHints (QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    m_pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_pScene->setView(m_pView);

    connect(m_pScene, SIGNAL(sigItemSelected(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)),
            this, SLOT(slotSceneItemSelected(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)));
    connect(m_pScene, &GraphicsScene::sigSceneClicked, [=](){
        hideOperatorForm();
    });
    connect(m_pScene, SIGNAL(sigItemResizeCompleted(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)),
            this, SLOT(slotSceneItemSelected(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)));
    connect(m_pScene, SIGNAL(sigItemPointsChanged(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)),
            this, SLOT(slotSceneItemSelected(int, TOOL_TYPE::Type, const QRect &, const QPointF &, const QPointF &)));
}

void MainWindow::slotActionGroup(QAction *action){
    m_eToolType = (TOOL_TYPE::Type)action->data().toInt();
    qDebug() << "slotActionGroup tool_type=" << m_eToolType;
    hideOperatorForm();
    m_pView->SetToolType(m_eToolType);
    m_pScene->setToolType(m_eToolType);
    m_pView->endMove();
}

QRect MainWindow::MapSceneToView(const QRect &rc){
    return m_pView->rect();
}

void MainWindow::slotSceneItemSelected(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2){
    showOperatorForm(key, toolType, rc, p1, p2);
}

void MainWindow::showOperatorForm(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2){
    if(nullptr == m_pOperatorForm){
        m_pOperatorForm = new OperatorForm(key, toolType, this);
        m_pOperatorForm->DisabledResize(true);
        connect(m_pOperatorForm, SIGNAL(sigPosChanged(int, int, int)),
                this, SLOT(slotItemPosChanged(int, int, int)));
        connect(m_pOperatorForm, SIGNAL(sigItemRemove(int)),
                this, SLOT(slotItemRemove(int)));
        connect(m_pOperatorForm, SIGNAL(sigResizeItemBegin(int)),
                this, SLOT(slotItemResizeBegin(int)));
        connect(m_pOperatorForm, SIGNAL(sigResizeItem(int, int, int)),
                this, SLOT(slotItemResize(int, int, int)));
        connect(m_pOperatorForm, SIGNAL(sigResizeItemEnd(int)),
                this, SLOT(slotItemResizeEnd(int)));
    }
    m_pOperatorForm->setKey(key);
    if(m_pOperatorForm->isHidden()){
        m_pOperatorForm->show();
    }
    QSize tsize = ui->toolbar->size();
    qreal itemX = rc.x();
    qreal itemY = rc.y()+tsize.height();
    qreal itemWidth = rc.width();
    qreal itemHeight = rc.height();
    qreal edge = OperatorForm::EDGE_WIDTH();
    QRect rectReal = QRect(itemX-edge, itemY-edge, itemWidth+edge*2, itemHeight+edge*2);
    m_pOperatorForm->setGeometry(rectReal);
    m_pOperatorForm->setToolType(toolType);
    if(TOOL_TYPE::LINE == toolType){
        QPoint p1_w = m_pOperatorForm->mapFromParent(p1.toPoint());
        QPoint p2_w = m_pOperatorForm->mapFromParent(p2.toPoint());
        p1_w.setY(p1_w.y()+tsize.height());
        p2_w.setY(p2_w.y()+tsize.height());
        m_pOperatorForm->setPoints(p1_w, p2_w);
    }
    m_pOperatorForm->showControls();
    m_pOperatorForm->raise();
}

void MainWindow::hideOperatorForm(){
    if(nullptr != m_pOperatorForm){
        m_pOperatorForm->hide();
    }
}

void MainWindow::slotItemPosChanged(int key, int dx, int dy){
    qreal scaled = m_pView->GetScale();
    qreal rdx = dx / scaled;
    qreal rdy = dy / scaled;
    m_pScene->onItemPosChanged(key, rdx, rdy);
}

void MainWindow::slotItemRemove(int key){
    m_pScene->onItemRemove(key);
    hideOperatorForm();
}


void MainWindow::slotItemResizeBegin(int key){
    m_pScene->onItemResizeBegin(key);
}

void MainWindow::slotItemResize(int key, int dx, int dy){
    qreal scaled = m_pView->GetScale();
    qreal rdx = dx / scaled;
    qreal rdy = dy / scaled;
    m_pScene->onItemResize(key, rdx, rdy);
    if(m_pOperatorForm != nullptr){
        QRect oldRect = m_pOperatorForm->geometry();
        QRect newRect = QRect(oldRect.x(), oldRect.y(), oldRect.width()+dx, oldRect.height()+dy);
        m_pOperatorForm->setGeometry(newRect);
    }
}

void MainWindow::slotItemResizeEnd(int key){
    m_pScene->onItemResizeEnd(key);
}

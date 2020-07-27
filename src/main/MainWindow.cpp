#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "src/graphics/GraphicsScene.h"
#include "src/graphics/GraphicsView.h"
#include "src/graphics/OperatorForm.h"

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
    });
    connect(ui->actionRedo, &QAction::triggered, [=](){
        qDebug() << "on redo";
    });

    connect(ui->actionMove, &QAction::triggered, [=](){
        qDebug() << "on move";
        m_pView->beginMove();
    });

    connect(ui->actionReset, &QAction::triggered, [=](){
        qDebug() << "on reset";
        m_pView->reset();
    });

    connect(ui->actionClear, &QAction::triggered, [=](){
        qDebug() << "on clear";
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

    connect(m_pScene, SIGNAL(sigItemSelected(int, const QRect &)), this, SLOT(slotSceneItemSelected(int, const QRect &)));
}

void MainWindow::slotActionGroup(QAction *action){
    m_eToolType = (TOOL_TYPE::Type)action->data().toInt();
    qDebug() << "slotActionGroup tool_type=" << m_eToolType;
    m_pScene->setToolType(m_eToolType);
    m_pView->endMove();
}

QRect MainWindow::MapSceneToView(const QRect &rc){
    //m_pView->rect();
}

void MainWindow::slotSceneItemSelected(int key, const QRect &rc){
    showOperatorForm(key, rc);
}

void MainWindow::showOperatorForm(int key, const QRect &rc){
    if(nullptr == m_pOperatorForm){
        m_pOperatorForm = new OperatorForm(key, this);
    }
    if(m_pOperatorForm->isHidden()){
        m_pOperatorForm->show();
        m_pOperatorForm->setGeometry(rc);
    }
}

void MainWindow::hideOperatorForm(){
    if(nullptr != m_pOperatorForm){
        m_pOperatorForm->hide();
    }
}

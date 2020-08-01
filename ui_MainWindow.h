/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSelect;
    QAction *actionPencil;
    QAction *actionRectangle;
    QAction *actionEllipse;
    QAction *actionLine;
    QAction *actionEraser;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionClear;
    QAction *actionZoomin;
    QAction *actionZoomout;
    QAction *actionReset;
    QAction *actionMove;
    QAction *actionTriangle;
    QWidget *centralwidget;
    QToolBar *toolbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionSelect->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/images/select_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resources/images/select_blue.png"), QSize(), QIcon::Disabled, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resources/images/select_gray.png"), QSize(), QIcon::Active, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resources/images/select_blue.png"), QSize(), QIcon::Active, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resources/images/select_gray.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resources/images/select_blue.png"), QSize(), QIcon::Selected, QIcon::On);
        actionSelect->setIcon(icon);
        actionPencil = new QAction(MainWindow);
        actionPencil->setObjectName(QString::fromUtf8("actionPencil"));
        actionPencil->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/images/brush_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/resources/images/brush_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionPencil->setIcon(icon1);
        actionRectangle = new QAction(MainWindow);
        actionRectangle->setObjectName(QString::fromUtf8("actionRectangle"));
        actionRectangle->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/images/rectangle_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resources/images/rectangle_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionRectangle->setIcon(icon2);
        actionEllipse = new QAction(MainWindow);
        actionEllipse->setObjectName(QString::fromUtf8("actionEllipse"));
        actionEllipse->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/images/circle_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/resources/images/circle_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionEllipse->setIcon(icon3);
        actionLine = new QAction(MainWindow);
        actionLine->setObjectName(QString::fromUtf8("actionLine"));
        actionLine->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/images/line_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/resources/images/line_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionLine->setIcon(icon4);
        actionEraser = new QAction(MainWindow);
        actionEraser->setObjectName(QString::fromUtf8("actionEraser"));
        actionEraser->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/images/eraser_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon5.addFile(QString::fromUtf8(":/resources/images/eraser_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionEraser->setIcon(icon5);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/images/revocation_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon6.addFile(QString::fromUtf8(":/resources/images/revocation_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUndo->setIcon(icon6);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/images/withdraw_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon7.addFile(QString::fromUtf8(":/resources/images/withdraw_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionRedo->setIcon(icon7);
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/images/clean_gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon8.addFile(QString::fromUtf8(":/resources/images/clean_blue.png"), QSize(), QIcon::Normal, QIcon::On);
        actionClear->setIcon(icon8);
        actionZoomin = new QAction(MainWindow);
        actionZoomin->setObjectName(QString::fromUtf8("actionZoomin"));
        actionZoomin->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/images/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon9.addFile(QString::fromUtf8(":/resources/images/zoom-in-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        actionZoomin->setIcon(icon9);
        actionZoomout = new QAction(MainWindow);
        actionZoomout->setObjectName(QString::fromUtf8("actionZoomout"));
        actionZoomout->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/images/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon10.addFile(QString::fromUtf8(":/resources/images/zoom-out-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        actionZoomout->setIcon(icon10);
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionReset->setCheckable(false);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/images/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon11.addFile(QString::fromUtf8(":/resources/images/reset-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        actionReset->setIcon(icon11);
        actionMove = new QAction(MainWindow);
        actionMove->setObjectName(QString::fromUtf8("actionMove"));
        actionMove->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/resources/images/move.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon12.addFile(QString::fromUtf8(":/resources/images/move-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        actionMove->setIcon(icon12);
        actionTriangle = new QAction(MainWindow);
        actionTriangle->setObjectName(QString::fromUtf8("actionTriangle"));
        actionTriangle->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/resources/images/triangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon13.addFile(QString::fromUtf8(":/resources/images/triangle-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        actionTriangle->setIcon(icon13);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        toolbar = new QToolBar(MainWindow);
        toolbar->setObjectName(QString::fromUtf8("toolbar"));
        toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolbar);

        toolbar->addAction(actionSelect);
        toolbar->addSeparator();
        toolbar->addAction(actionPencil);
        toolbar->addAction(actionRectangle);
        toolbar->addAction(actionEllipse);
        toolbar->addAction(actionTriangle);
        toolbar->addAction(actionLine);
        toolbar->addSeparator();
        toolbar->addAction(actionEraser);
        toolbar->addSeparator();
        toolbar->addAction(actionUndo);
        toolbar->addAction(actionRedo);
        toolbar->addSeparator();
        toolbar->addAction(actionMove);
        toolbar->addSeparator();
        toolbar->addAction(actionZoomin);
        toolbar->addAction(actionZoomout);
        toolbar->addAction(actionReset);
        toolbar->addSeparator();
        toolbar->addAction(actionClear);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSelect->setText(QCoreApplication::translate("MainWindow", "Select", nullptr));
        actionPencil->setText(QCoreApplication::translate("MainWindow", "Pencil", nullptr));
        actionRectangle->setText(QCoreApplication::translate("MainWindow", "Rectangle", nullptr));
        actionEllipse->setText(QCoreApplication::translate("MainWindow", "Ellipse", nullptr));
        actionLine->setText(QCoreApplication::translate("MainWindow", "Line", nullptr));
        actionEraser->setText(QCoreApplication::translate("MainWindow", "Eraser", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        actionClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        actionZoomin->setText(QCoreApplication::translate("MainWindow", "Zoomin", nullptr));
        actionZoomout->setText(QCoreApplication::translate("MainWindow", "Zoomout", nullptr));
        actionReset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        actionMove->setText(QCoreApplication::translate("MainWindow", "Move", nullptr));
        actionTriangle->setText(QCoreApplication::translate("MainWindow", "Triangle", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

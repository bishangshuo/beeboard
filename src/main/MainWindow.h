#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/common/ToolType.h"
#include  <QActionGroup>

class GraphicsScene;
class GraphicsView;
class OperatorForm;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotActionGroup(QAction *action);

protected:
    void resizeEvent(QResizeEvent* event);
private:
    void setupActions();
    void initGraphics();

    void OpenBoardFile();
private:
    Ui::MainWindow *ui;
    TOOL_TYPE::Type m_eToolType;

    QActionGroup *m_pActionGroup;
    GraphicsScene *m_pScene;
    GraphicsView *m_pView;
};
#endif // MAINWINDOW_H

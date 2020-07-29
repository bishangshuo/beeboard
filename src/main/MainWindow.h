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
    void slotSceneItemSelected(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2);
    void slotItemPosChanged(int key, int dx, int dy);
    void slotItemRemove(int key);

    void slotItemResizeBegin(int key);
    void slotItemResize(int key, int dx, int dy);
    void slotItemResizeEnd(int key);

    void slotItemRotateBegin(int key);
    void slotItemRotate(int key, qreal angle);
    void slotItemRotateEnd(int key);
protected:
    void resizeEvent(QResizeEvent* event);
private:
    void setupActions();
    void initGraphics();
    void showOperatorForm(int key, TOOL_TYPE::Type toolType, const QRect &rc, const QPointF &p1, const QPointF &p2);
    void hideOperatorForm();
    QRect MapSceneToView(const QRect &rc);
private:
    Ui::MainWindow *ui;
    TOOL_TYPE::Type m_eToolType;

    QActionGroup *m_pActionGroup;
    GraphicsScene *m_pScene;
    GraphicsView *m_pView;
    OperatorForm *m_pOperatorForm;
};
#endif // MAINWINDOW_H

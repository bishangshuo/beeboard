#include "GraphicsView.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QScrollBar>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      m_translateButton(Qt::LeftButton),
      m_scale(1.0),
      m_zoomDelta(0.1),
      m_translateSpeed(1.0),
      m_bMouseTranslate(false)
{
    // 去掉滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    centerOn(0, 0);
}

// 平移速度
void GraphicsView::setTranslateSpeed(qreal speed)
{
    // 建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "GraphicsView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}

qreal GraphicsView::translateSpeed() const
{
    return m_translateSpeed;
}

// 缩放的增量
void GraphicsView::setZoomDelta(qreal delta)
{
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "GraphicsView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

qreal GraphicsView::zoomDelta() const
{
    return m_zoomDelta;
}

// 上/下/左/右键向各个方向移动、加/减键进行缩放、空格/回车键旋转
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        translate(QPointF(0, -2));  // 上移
        break;
    case Qt::Key_Down:
        translate(QPointF(0, 2));  // 下移
        break;
    case Qt::Key_Left:
        translate(QPointF(-2, 0));  // 左移
        break;
    case Qt::Key_Right:
        translate(QPointF(2, 0));  // 右移
        break;
    case Qt::Key_Plus:  // 放大
        zoomIn();
        break;
    case Qt::Key_Minus:  // 缩小
        zoomOut();
        break;
    case Qt::Key_Space:  // 逆时针旋转
        rotate(-5);
        break;
    case Qt::Key_Enter:  // 顺时针旋转
    case Qt::Key_Return:
        rotate(5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int sx = verticalScrollBar()->x();
    int sy = verticalScrollBar()->y();
    //qDebug() << "GraphicsView::mousePressEvent pos = " << pos << " sx=" << sx << "sy" << sy;

    if(TOOL_TYPE::MOVE == m_eToolType || TOOL_TYPE::ZOOMIN == m_eToolType || TOOL_TYPE::ZOOMOUT == m_eToolType){
           if (event->button() == m_translateButton && bMove) {
            m_bMouseTranslate = true;
            m_lastMousePos = pos;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

// 平移
void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(TOOL_TYPE::MOVE == m_eToolType || TOOL_TYPE::ZOOMIN == m_eToolType || TOOL_TYPE::ZOOMOUT == m_eToolType){
        if (m_bMouseTranslate && bMove){
            QPointF mouseDelta = mapToScene(pos) - mapToScene(m_lastMousePos);
            translate(mouseDelta);
        }

        m_lastMousePos = pos;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(TOOL_TYPE::MOVE == m_eToolType || TOOL_TYPE::ZOOMIN == m_eToolType || TOOL_TYPE::ZOOMOUT == m_eToolType){
        if (event->button() == m_translateButton && bMove)
            m_bMouseTranslate = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

// 放大/缩小
void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

// 放大
void GraphicsView::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

// 缩小
void GraphicsView::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

// 缩放 - scaleFactor：缩放的比例因子
void GraphicsView::zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    m_scale *= scaleFactor;
}

// 平移
void GraphicsView::translate(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= m_scale;
    delta *= m_translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

void GraphicsView::reset(){
    resetPos();
    resetZoom();
}

void GraphicsView::resetPos(){
    centerOn(0, 0);
}

void GraphicsView::resetZoom(){
    resetMatrix();
    zoom(1);
    m_scale = 1.0;
    m_zoomDelta = 0.1;
    m_translateSpeed = 1.0;
}

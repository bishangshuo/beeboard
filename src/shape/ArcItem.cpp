#include "ArcItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include "src/property/PropObj.h"
#include <QDebug>

inline qreal checkAngle(qreal _angle){
    qreal angle = _angle;
    if (angle > 360.0)
    {
        while(1)
        {
            angle -= 360.0;
            if (angle < 360.0) break;
        }
    }
    else if (angle < 0.0)
    {
        while(1)
        {
            angle += 360.0;
            if (angle > 0.0) break;
        }
    }
    return angle;
}

ArcItem::ArcItem(int x, int y, int width, int height)
    : QGraphicsEllipseItem(x, y, width, height)
    , m_rcOrg(x, y, width, height)
    , m_isRotating(false)
    , m_isCreating(true)
    , m_hideRotate(false)
    , m_hideClose(false)

{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);

    setPos(x+width/2, y+height/2);
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rc;
    if(m_isCreating){
        rc = m_rcOrg;
    }else{
        rc = boundingRect();
    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(m_pen);
    painter->drawArc(rc, startAngle(), spanAngle());

    QColor color = Qt::green;
    QPen p1(QBrush(color), 1);
    p1.setStyle(Qt::DashDotLine);
    painter->setPen(p1);

    if (option->state & QStyle::State_Selected)
    {
        //绘制选中边框
        {
            painter->drawRect(m_rcOrg);
        }

        //绘制饼状连接线
        {
            int penWidth = m_pen.width();
            QRectF rcs(rc.x()-penWidth/2,rc.y()-penWidth/2,rc.width()+penWidth,rc.height()+penWidth);
            QPainterPath path;
            path.moveTo(0,0);
            path.arcTo(rcs, startAngle()/16, spanAngle()/16);
            path.lineTo(0,0);
            painter->drawPath(path);

        }

        int width = m_rcOrg.width();
        int height = m_rcOrg.height();

        if(!m_hideRotate){
            //旋转按钮区域
            QPixmap px_r(":/resources/images/rotate-left.png");
            px_r = px_r.scaled(20, 20);
            painter->drawPixmap(-10, -height/2, px_r);
        }

        if(!m_hideClose){
            //关闭按钮区域
            QPixmap px_c(":/resources/images/close.png");
            px_c = px_c.scaled(20, 20);
            painter->drawPixmap(width/2-20, -height/2, px_c);
        }

    }
}

QPainterPath ArcItem::shape() const {
    QPainterPath path;
    QRectF rc;
    int penWidth = m_pen.width();
    rc = m_rcOrg;
    if(m_isCreating){

        path.moveTo(rc.topLeft());
        path.lineTo(rc.topRight());
        path.lineTo(rc.bottomRight());
        path.lineTo(rc.bottomLeft());
        path.lineTo(rc.topLeft());
    }else{
        QRectF rcs(rc.x()-penWidth/2,rc.y()-penWidth/2,rc.width()+penWidth,rc.height()+penWidth);
        path.moveTo(0, 0);
        path.arcTo(rcs, startAngle()/16, spanAngle()/16);
        path.lineTo(0, 0);
    }

    int width = m_rcOrg.width();
    int height = m_rcOrg.height();

    //添加操作按钮，以便选择区域
    //求出按钮减去扇形的差集，如果结果集不为空，则加入形状集合

    //旋转按钮
    QPainterPath rotatePath;
    rotatePath.addEllipse(-10, -height/2, 20, 20);
    QPainterPath addedRotatePath = rotatePath.subtracted(path);
    if(!addedRotatePath.isEmpty()){
        path.addPath(addedRotatePath);
    }

    //关闭按钮
    QPainterPath closePath;
    closePath.addEllipse(width/2-20, -height/2, 20, 20);
    QPainterPath addedClosePath = closePath.subtracted(path);
    if(!addedClosePath.isEmpty()){
        path.addPath(addedClosePath);
    }

    return path;
}

QRectF ArcItem::boundingRect() const{
    return m_rcOrg;
}

void ArcItem::Created(){
    if(m_isCreating){
        m_isCreating = false;
        QPointF pos = scenePos();
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), 0, 0, 0));
    }
}

void ArcItem::SetPos(const QPointF &pos){
    setPos(pos);
}

void ArcItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ((isInRotateArea(event->pos()) && isSelected()) && !m_hideRotate)
    {
        QCursor *rotateCursor = new QCursor(QPixmap(":/resources/images/rotate-left-cur.png"));
        setCursor(*rotateCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void ArcItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_ptClicked = event->scenePos();
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInRotateArea(event->pos())  && !m_hideRotate)
    {
        m_isRotating = true;
    }
    else if (event->button() == Qt::LeftButton && isInCloseArea(event->pos())  && !m_hideClose)
    {
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }else{
        QGraphicsItem::mousePressEvent(event);
    }
}

void ArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isRotating && !m_hideRotate)
    {
        int width = m_rcOrg.width();
        QPointF cursorPos = event->pos();
        QVector2D vectorStart = QVector2D(QPointF(0.0, -width / 2) - QPointF(0.0, 0.0));           // 起始向量
        QVector2D vectorEnd = QVector2D(cursorPos - QPointF(0.0, 0.0));                             // 结束向量
        // 计算起始向量和结束向量之间的角度
        qreal angle = 0.0;
        qreal angleEnd = GetDegreeAngle(vectorEnd);
        qreal angleStart = GetDegreeAngle(vectorStart);
        angle = angleEnd - angleStart + rotation();
        Rotate(angle);

        if(m_pCBRotate){
            QPointF _scenePos = scenePos();
            m_pCBRotate(_scenePos.x(), _scenePos.y(), angle);
        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isRotating)
    {
        m_isRotating = false;
    }
    else
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }

    int width = m_rcOrg.width();
    int height = m_rcOrg.height();

    QPointF pos = scenePos();
    BASEITEM_GEO *geo = new BASEITEM_GEO(pos.x(), pos.y(), width, height, rotation());
    BASEITEM_GEO *topGeo = m_stUndo.top();
    if(!topGeo->compare(*geo)){
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), width, height, rotation()));
    }

    if(m_ptClicked != event->scenePos()){
        if(m_pCBItemChanged){
            m_pCBItemChanged(reinterpret_cast<int>(this));
        }
    }
}

int ArcItem::type() const
{
    return Type;
}

bool ArcItem::isInRotateArea(const QPointF &pos) const
{
    return QRectF(-10, -m_rcOrg.height()/2, 20, 20).contains(pos);
}

bool ArcItem::isInCloseArea(const QPointF &pos) const
{
    return QRectF(m_rcOrg.width()/2-20, -m_rcOrg.height()/2, 20, 20).contains(pos);
}

void ArcItem::Rotate(qreal _angle){
    qreal angle = checkAngle(_angle);
    setRotation(angle);
}

void ArcItem::Undo(){
    if(m_stUndo.size() < 2){
        return;
    }
    BASEITEM_GEO *geo_r = m_stUndo.pop();

    BASEITEM_GEO *geo = m_stUndo.top();
    QPointF pos = QPointF(geo->px, geo->py);
    qreal angle = geo->angle;
    setPos(pos);
    Rotate(angle);
    update();

    m_stRedo.push(geo_r);
}

void ArcItem::Redo(){
    if(m_stRedo.size() == 0){
        return;
    }
    BASEITEM_GEO *geo = m_stRedo.pop();
    QPointF pos = QPointF(geo->px, geo->py);
    qreal angle = geo->angle;
    setPos(pos);
    Rotate(angle);
    update();
    m_stUndo.push(geo);
}


#include "LineItem.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include "src/property/PropObj.h"
#include <QDebug>

const int RZICON_LEN = 20;
const int EDGE_LEN   = 10;

inline QPointF rzPos(const QPointF &p1, const QPointF &p2){
    // p2与p1都在x轴
    if(p2.toPoint().y() == p1.toPoint().y()){
        if(p2.x() < p1.x()){// p2在左，p1在右
            return p2+QPointF(RZICON_LEN, 0);
        }else{// p2在右，p1在左
            return p2-QPointF(RZICON_LEN, 0);
        }
    }
    // p2与p1都在y轴
    if(p2.toPoint().x() == p1.toPoint().x()){
        if(p2.y() < p1.y()){// p2在上，p1在下
            return p2+QPointF(0, RZICON_LEN);
        }else{// p2在下，p1在上
            return p2-QPointF(0, RZICON_LEN);
        }
    }

    qreal m = RZICON_LEN;//p0与p2的距离
    qreal r = (p1.y()-p2.y())  / (p1.x()-p2.x());//斜率
    //p2-p0向量与p2-p1同方向
    //p2-p1东北方向
    qreal x0 = p2.x(), y0 = p2.y();
    if(p1.x()>p2.x() && p1.y()<p2.y()){
        x0 = sqrt(m*m/(1+r*r)) + p2.x();
        y0 = -sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()>p2.x() && p1.y()>p2.y()){//东南方向
        x0 = sqrt(m*m/(1+r*r)) + p2.x();
        y0 = sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()<p2.x() && p1.y()>p2.y()){//西南方向
        x0 = -sqrt(m*m/(1+r*r)) + p2.x();
        y0 = sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }else if(p1.x()<p2.x() && p1.y()<p2.y()){//西北方向
        x0 = -sqrt(m*m/(1+r*r)) + p2.x();
        y0 = -sqrt(m*m*r*r/(1+r*r)) + p2.y();
    }
    return QPointF(x0, y0);
}

LineItem::LineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
    , m_pressed(false)
    , m_isResizing(false)
    , m_hideClose(false)
    , m_hideResize(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
}

LineItem::~LineItem(){
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setAcceptHoverEvents(false);
    m_pressed = false;
    m_isResizing = false;
    m_hideClose = false;
    m_hideResize = false;
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //QGraphicsLineItem::paint(painter, option, widget);
    QPointF p1 = line().p1();
    QPointF p2 = line().p2();
    painter->setPen(m_pen);
    painter->drawLine(QLineF(p1, p2));

    QColor color(255, 0, 0, 128);

    if(isSelected()){
       QPen p;
       p.setColor(Qt::blue);
       painter->setPen(p);
       painter->drawPoint(p1);

       //绘制选中边框
       {
           QPen pen_r = painter->pen();
           pen_r.setColor(color);
           pen_r.setWidth(1);
           pen_r.setStyle(Qt::DashLine);
           painter->setPen(pen_r);
           //选中边框
           qreal width = abs(p2.x()-p1.x());
           qreal height = abs(p2.y()-p1.y());
           QPointF ptMid = (p1+p2)/2;
           QPainterPath path = shape();
           painter->drawPath(path);
//           painter->drawRect(ptMid.x()-width/2-1, ptMid.y()-height/2-1,  width+2, height+2);
       }

       if(!m_hideClose){
           QPixmap px_c(":/resources/images/close.png");
           px_c = px_c.scaled(RZICON_LEN, RZICON_LEN);
           QPointF ptClose = (p1+p2)/2;
           painter->drawPixmap(ptClose.x()-RZICON_LEN/2, ptClose.y()-RZICON_LEN/2, QPixmap(px_c));
       }

       if(!m_hideResize){
           QPixmap px_r(":/resources/images/move.png");
           px_r = px_r.scaled(RZICON_LEN, RZICON_LEN);
           QPointF ptMove = rzPos(p1, p2);
           painter->drawPixmap(ptMove.x()-RZICON_LEN/2, ptMove.y()-RZICON_LEN/2, QPixmap(px_r));
       }
    }
}

QPainterPath LineItem::shape() const{
    QPainterPath path;

    QPointF p1_org = line().p1();
    QPointF p2_org = line().p2();

    //中心点
    QPointF pMid = (p1_org + p2_org)/2;

    //坐标原点
    QPointF pOrg = QPointF(0, 0);

    //平移向量
    qreal dx = pOrg.x() - pMid.x();
    qreal dy = pOrg.y() - pMid.y();

    //平移直线
    QPointF p1 = p1_org + QPointF(dx, dy);
    QPointF p2 = p2_org + QPointF(dx, dy);

    QPointF c0, c1, c2, c3;
    qreal a = atan(abs(p1.y()/p1.x()));
    qreal p1_0 = sqrt(p1.x()*p1.x()+p1.y()*p1.y());
    qreal b = atan(EDGE_LEN/p1_0);
    qreal c_0 = sqrt(EDGE_LEN*EDGE_LEN+p1_0*p1_0);

    //p1在第二象限, p2在第四象限
    if(p1.x() < 0 && p1.y() < 0 && p2.x() > 0 && p2.y() > 0){

        //C1
        qreal c = a+b;
        qreal x_abs = c_0*cos(c);
        qreal y_abs = c_0*sin(c);
        c1 = QPointF(-x_abs, -y_abs);

        //C0
        c = a-b;
        x_abs = c_0*cos(c);
        y_abs = c_0*sin(c);
        c0 = QPointF(-x_abs, -y_abs);
    }else if(p1.x() > 0 && p1.y() <0 && p2.x() < 0 && p2.y() > 0){
        qreal c = a-b;
        qreal x_abs = c_0*cos(c);
        qreal y_abs = c_0*sin(c);
        c1 = QPointF(x_abs, -y_abs);

        c = a+b;
        x_abs = c_0*cos(c);
        y_abs = c_0*sin(c);
        c0 = QPointF(x_abs, -y_abs);
    }else if(p1.x() > 0 && p1.y() > 0 && p2.x() < 0 && p2.y() < 0){
        qreal c = a+b;
        qreal x_abs = c_0*cos(c);
        qreal y_abs = c_0*sin(c);
        c1 = QPointF(x_abs, y_abs);

        c = a-b;
        x_abs = c_0*cos(c);
        y_abs = c_0*sin(c);
        c0 = QPointF(x_abs, y_abs);
    }else if(p1.x() < 0 && p1.y() > 0 && p2.x() > 0 && p2.y() < 0){
        qreal c = a-b;
        qreal x_abs = c_0*cos(c);
        qreal y_abs = c_0*sin(c);
        c1 = QPointF(-x_abs, y_abs);

        c = a+b;
        x_abs = c_0*cos(c);
        y_abs = c_0*sin(c);
        c0 = QPointF(-x_abs, y_abs);
    }else if(p1.x() == 0 && p2.x() == 0){
        if(p1.y() < 0 && p2.y() > 0){
            c1 = QPointF(EDGE_LEN, p1.y());
            c0 = QPointF(-EDGE_LEN, p1.y());
        }else{
            c1 = QPointF(-EDGE_LEN, p1.y());
            c0 = QPointF(EDGE_LEN, p1.y());
        }
    }else if(p1.y() == 0 && p2.y() == 0){
        if(p1.x() < 0 && p2.x() > 0){
            c1 = QPointF(p1.x(), -EDGE_LEN);
            c0 = QPointF(p1.x(), EDGE_LEN);
        }else{
            c1 = QPointF(p1.x(), EDGE_LEN);
            c0 = QPointF(p1.x(), -EDGE_LEN);
        }
    }

    c3 = QPointF(-c1.x(), -c1.y());
    c2 = QPointF(-c0.x(), -c0.y());


    //平移回"原来"位置
    c0 = c0+QPointF(-dx, -dy);
    c1 = c1+QPointF(-dx, -dy);
    c2 = c2+QPointF(-dx, -dy);
    c3 = c3+QPointF(-dx, -dy);

    path.moveTo(c0);
    path.lineTo(c1);
    path.lineTo(c2);
    path.lineTo(c3);
    path.lineTo(c0);

    return path;
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event){

    m_pressed = true;
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInResizeArea(event->pos()) && isSelected())
    {
        m_isResizing = true;
        m_beginSizePoint = event->pos();
    }
    else if (event->button() == Qt::LeftButton && isInCloseArea(event->pos()) && isSelected())
    {
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }
    //QGraphicsItem::mousePressEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){

    if(isSelected() && m_isResizing){
        QPointF deltaPt = event->pos()-m_beginSizePoint;
        QPointF p1 = line().p1();
        QPointF p2 = line().p2();
        qDebug() << "LineItem::mouseMoveEvent before p1=" <<p1<<", p2="<<p2;
        qDebug() << "LineItem::mouseMoveEvent deltaPt=" <<deltaPt;
        p2 = p2+deltaPt;
        setLine(QLineF(p1, p2));
        qDebug() << "LineItem::mouseMoveEvent after p1=" <<p1<<", p2="<<p2;
        m_beginSizePoint = event->pos();
    }else{
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseReleaseEvent(event);
    m_pressed = false;
    m_isResizing = false;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void LineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ((isInResizeArea(event->pos()) && isSelected())){
        setCursor(Qt::SizeAllCursor);
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }else{
        unsetCursor();
        setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

bool LineItem::isInResizeArea(const QPointF &pos) const
{
    QPointF p1 = line().p1();
    QPointF p2 = line().p2();
    QPointF ptMove = rzPos(p1, p2);
    bool b = QRectF(ptMove.x()-RZICON_LEN/2, ptMove.y()-RZICON_LEN/2, RZICON_LEN, RZICON_LEN).contains(pos);
    if(b){
        //qDebug() << "isInResizeArea";
    }
    return b;
}

bool LineItem::isInCloseArea(const QPointF &pos) const
{
    QPointF p1 = line().p1();
    QPointF p2 = line().p2();
    QPointF ptClose = (p1+p2)/2;
    bool b = QRectF(ptClose.x()-RZICON_LEN/2, ptClose.y()-RZICON_LEN/2, RZICON_LEN, RZICON_LEN).contains(pos);
    if(b){
        //qDebug() << "isInCloseArea";
    }
    return b;
}

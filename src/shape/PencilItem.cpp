#include "PencilItem.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <qmath.h>
#include <QDebug>
#include <QRandomGenerator>
#include <QtConcurrent>
#include <QPainterPathStroker>
#include <QDateTime>
#include "src/property/PropObj.h"

PencilItem::PencilItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , m_isCreating(true)
    , m_hideClose(false)

{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    m_pen.setWidth(PropObj::GetInstance()->PenWidth());
    m_pen.setColor(PropObj::GetInstance()->PenColor());
    m_pen.setJoinStyle(PropObj::GetInstance()->PenJoinStyle());
    m_pen.setCapStyle(PropObj::GetInstance()->PenCapStyle());
    setPen(m_pen);
}

PencilItem::~PencilItem(){
    if(m_mapEraserPathUndo.size() > 0){
        for(MapEraserPath::iterator it = m_mapEraserPathUndo.begin(); it != m_mapEraserPathUndo.end(); ){
            VectorPath *vectorPath = it.value();
            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); ){
                ERASER_PATH *data = *itV;
                delete data;
                itV = vectorPath->erase(itV);
            }
            vectorPath->clear();
            delete vectorPath;
            it = m_mapEraserPathUndo.erase(it);
        }
    }
}

void PencilItem::Created(){
    if(m_isCreating){
        m_isCreating = false;
        mPath = path();
        QPointF pos = scenePos();
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), 0, 0, 0));
    }
}

QPainterPath PencilItem::shape() const{
    QRectF rc = path().boundingRect();
    QPainterPath shapePath;
    shapePath.moveTo(rc.topLeft());
    shapePath.lineTo(rc.topRight());
    shapePath.lineTo(rc.bottomRight());
    shapePath.lineTo(rc.bottomLeft());
    shapePath.lineTo(rc.topLeft());
    return shapePath;
}

//QRectF PencilItem::boundingRect() const{
//    QRectF rc = path().boundingRect();
//    QRectF bRc = QRectF(rc.x()-1, rc.y()-1, rc.width()+2, rc.height()+2);
//    return bRc;
//}

void PencilItem::setEraser(int key, QPainterPath *path, int width){

}

void PencilItem::RenderPathToPixmap(){

    QPainterPath orgPath = path();

    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.width());
    QPainterPath strokerPath = stroker.createStroke(orgPath);

    QRectF rc = strokerPath.boundingRect();
    QPixmap pixmap(rc.width(), rc.height());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    //绘制原来path到pixmap
    painter.setPen(m_pen);
    painter.translate(-rc.topLeft());
    painter.drawPath(orgPath);

    painter.end();

    m_pixmap = pixmap.copy();
}

void PencilItem::RenderEraserToPixmap(){

    if(m_mapEraserPathUndo.size() <= 0){
        return;
    }

    QPainterPath orgPath = path();

    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.width());
    QPainterPath strokerPath = stroker.createStroke(orgPath);

    QRectF rc = strokerPath.boundingRect();

    QPixmap pixmap = m_pixmap.copy();

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    painter.translate(-rc.topLeft());

    //绘制橡皮擦到pixmap
    if(m_mapEraserPathUndo.size() > 0){
        for(MapEraserPath::iterator it = m_mapEraserPathUndo.begin(); it != m_mapEraserPathUndo.end(); it ++){
            VectorPath *vectorPath = it.value();
            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); itV++){
                ERASER_PATH *path_data = *itV;
                QPainterPath *path = path_data->path;
                if(path->elementCount() > 1 && !path_data->drawn){
                    int width = path_data->width;
                    QPen pen;
                    pen.setWidth(width);
                    pen.setCapStyle(Qt::RoundCap);
                    pen.setJoinStyle(Qt::RoundJoin);
                    pen.setColor(Qt::white);
                    painter.setPen(pen);

                    QPainter::CompositionMode oldMode = painter.compositionMode();
                    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
                    painter.drawPath(*path);
                    painter.setCompositionMode(oldMode);
                }
            }
        }
    }

    painter.end();

    m_pixmap = pixmap.copy();
}

void PencilItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing, true);

    if(m_isCreating){
        RenderPathToPixmap();
    }

    RenderEraserToPixmap();

    if(!m_pixmap.isNull()){
        QPen p = pen();
        QPainterPathStroker stroker;
        stroker.setWidth(p.width());
        QPainterPath strokerPath = stroker.createStroke(path());
        QRectF rc = strokerPath.boundingRect();
        painter->drawPixmap(rc, m_pixmap, QRectF(0, 0, rc.width(), rc.height()));
    }

    QRectF rc = path().boundingRect();
    QColor color(255, 0, 0, 128);

    if (option->state & QStyle::State_Selected)
    {
        //绘制选中边框
        {
            QPen pen_r;
            pen_r.setColor(color);
            pen_r.setWidth(1);
            pen_r.setStyle(Qt::DashLine);
            painter->setPen(pen_r);
            painter->drawRect(rc);
        }

        if(!m_hideClose)
        {
            //关闭按钮区域
            QPixmap px_c(":/resources/images/close.png");
            px_c = px_c.scaled(20, 20);
            painter->drawPixmap(rc.topRight()-QPointF(20, 0), px_c);
        }

    }

    //不要调用父类的paint，因为path已经自绘，否则出现默认黑色选择框
    //QGraphicsPathItem::paint(painter, option, widget);
}

void PencilItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
}

void PencilItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void PencilItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_ptClicked = event->scenePos();
    static qreal z = 0.0;
    setZValue(z += 1.0);
    if (event->button() == Qt::LeftButton && isInCloseArea(event->pos())  && !m_hideClose)
    {
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }else{
        QGraphicsItem::mousePressEvent(event);
    }
}

void PencilItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    QPointF pos = scenePos();
    BASEITEM_GEO *geo = new BASEITEM_GEO(pos.x(), pos.y(), 0, 0, 0);
    BASEITEM_GEO *topGeo = m_stUndo.top();
    if(!topGeo->compare(*geo)){
        m_stUndo.push(new BASEITEM_GEO(pos.x(), pos.y(), 0, 0, 0));
    }

    if(m_ptClicked != event->scenePos()){
        if(m_pCBItemChanged){
            m_pCBItemChanged(reinterpret_cast<int>(this));
        }
    }
}

bool PencilItem::isInCloseArea(const QPointF &pos) const
{
    QRectF rc = path().boundingRect();
    QPointF pt = rc.topRight();
    return QRectF(pt.x()-20, pt.y(), 20, 20).contains(pos);
}

//遍历image所有点，如果发现有不透明的点，则说明不可移除
void PencilItem::onEraserRelease(){

    if(m_mapEraserPathUndo.size() > 0){
        for(MapEraserPath::iterator it = m_mapEraserPathUndo.begin(); it != m_mapEraserPathUndo.end(); it ++){
            VectorPath *vectorPath = it.value();
            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); itV++){
                if(!(*itV)->drawn){
                    (*itV)->drawn = true;
                }
            }
        }
    }

    int key = reinterpret_cast<int>(this);
    QPixmap *pPixmap = new QPixmap();
    *pPixmap = m_pixmap.copy();
    QImage img = pPixmap->toImage();
    QRgb *st = (QRgb *) img.bits();
    quint64 pixelCount = img.width() * img.height();
    bool b = false;

    qint64 bt = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "end scan:"<<bt;
    for (quint64 p = 0; p < pixelCount; p++) {
        //st[p] = QColor(255, 128, 0).rgb();
        QRgb rgb = st[p];
        int alpha = qAlpha(rgb);
        if(alpha > 32){
            qDebug()<<"alpha="<<alpha;
            b = true;
            break;
        }
    }
    delete pPixmap;
    qint64 et = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "end scan:"<<et<<", erasetime="<<et-bt;

    if(!b){
        if(m_pCBRemove){
            m_pCBRemove(reinterpret_cast<int>(this));
        }
    }
}

void PencilItem::Undo(){
    if(m_stUndo.size() < 2){
        return;
    }
    BASEITEM_GEO *geo_r = m_stUndo.pop();

    BASEITEM_GEO *geo = m_stUndo.top();
    QPointF pos = QPointF(geo->px, geo->py);
    setPos(pos);
    update();

    m_stRedo.push(geo_r);
}

void PencilItem::Redo(){
    if(m_stRedo.size() == 0){
        return;
    }
    BASEITEM_GEO *geo = m_stRedo.pop();
    QPointF pos = QPointF(geo->px, geo->py);
    setPos(pos);
    update();
    m_stUndo.push(geo);
}

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

PencilItem::PencilItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , m_isCreating(true)
    , m_hideClose(false)
    , m_cbRemove(NULL)

{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
}

PencilItem::~PencilItem(){

}

void PencilItem::Created(){
    m_isCreating = false;
    mPath = path();
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

void PencilItem::setEraser(int key, QPainterPath *path, int width){

}

void PencilItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen p = pen();
    painter->setPen(p);
    painter->drawPath(path());

    //绘制橡皮擦
    if(m_mapEraserPathUndo.size() > 0){
        QPainter::CompositionMode oldMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        for(MapEraserPath::iterator it = m_mapEraserPathUndo.begin(); it != m_mapEraserPathUndo.end(); it ++){
            VectorPath *vectorPath = it.value();
            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); itV++){
                ERASER_PATH *path_data = *itV;
                int width = path_data->width;
                QPainterPath *path = path_data->path;
                QPen pen;
                pen.setWidth(width);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                pen.setColor(Qt::white);
                painter->setPen(pen);
                if(path->elementCount() > 1){
                    painter->drawPath(*path);
                }
            }
        }

        painter->setCompositionMode(oldMode);
    }

    QRectF rc = path().boundingRect();
    QColor color(255, 0, 0, 128);

    if (option->state & QStyle::State_Selected)
    {
        //绘制选中边框
        //if (option->state & QStyle::State_HasFocus)
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
}

bool PencilItem::isInCloseArea(const QPointF &pos) const
{
    QRectF rc = path().boundingRect();
    QPointF pt = rc.topRight();
    return QRectF(pt.x()-20, pt.y(), 20, 20).contains(pos);
}

void PencilItem::onEraserRelease(){
    //将所有path画到pixmap, 但不要包括选择焦点边框和关闭按钮
    QRectF rc = boundingRect();
    QPixmap pixmap(rc.width(), rc.height());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QPen p = pen();
    painter.setPen(p);
    painter.drawPath(mPath);

    //绘制橡皮擦
    if(m_mapEraserPathUndo.size() > 0){
        //QPainter::CompositionMode oldMode = painter.compositionMode();
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        for(MapEraserPath::const_iterator it = m_mapEraserPathUndo.constBegin(); it != m_mapEraserPathUndo.constEnd(); it ++){
            VectorPath *vectorPath = it.value();
            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); itV++){
                ERASER_PATH *path_data = *itV;
                int width = path_data->width;
                QPainterPath *path = path_data->path;
                QPen pen;
                pen.setWidth(width);
                pen.setCapStyle(Qt::RoundCap);
                pen.setJoinStyle(Qt::RoundJoin);
                pen.setColor(Qt::white);
                painter.setPen(pen);
                if(path->elementCount() > 1){
                    painter.drawPath(*path);
                }
            }
        }

        //painter.setCompositionMode(oldMode);
    }

    painter.end();

    int key = reinterpret_cast<int>(this);
    QPixmap *pPixmap = new QPixmap();
    *pPixmap = pixmap.copy();
    QtConcurrent::run([this, key, pPixmap](){
        QImage img = pPixmap->toImage();
        QRgb *st = (QRgb *) img.bits();
        quint64 pixelCount = img.width() * img.height();

        bool b = false;
        for (quint64 p = 0; p < pixelCount; p++) {
            //st[p] = QColor(255, 128, 0).rgb();
            QRgb rgb = st[p];
            int alpha = qAlpha(rgb);
            if(alpha > 0){
                qDebug()<<"alpha="<<alpha;
                b = true;
                //break;
            }
        }
        if(!b){
            if(m_cbRemove != NULL){
                m_cbRemove(key);
            }
        }
        delete pPixmap;
    });
}

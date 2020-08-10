#include "Pencil.h"
#include "src/graphics/GraphicsScene.h"
#include <QPainter>
#include <QDebug>
#include "Eraser.h"
#include <QStyleOptionGraphicsItem>
#include <QtConcurrent>

static QPixmap QPixmapFromItem(QGraphicsItem *item){
    QPixmap pixmap(item->boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOptionGraphicsItem opt;
    item->paint(&painter, &opt);
    return pixmap;
}

Pencil::Pencil(QObject *parent)
    : ShapeBase(parent)
{

}

int Pencil::Create(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) {
    m_pItem = new PencilItem();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    m_pItem->setPen(pen);
    pScene->addItem(m_pItem);
    int key = reinterpret_cast<int>(m_pItem);
    m_pItem->setData(ITEM_DATA_KEY, key);

    m_pItem->SetRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });

    m_pItem->setShouldRemoveCallback([=](int _key){
        emit sigRemove(_key);
    });

    m_path.moveTo(leftTop);
    m_path.lineTo(leftTop+QPointF(1,1));
    m_point = leftTop;
    m_pItem->setPath(m_path);

    QRectF rc = m_path.boundingRect();
    qDebug() << "Pencil::UpdateRect rect(path)="<<rc;

    return key;
}
void Pencil::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene) {

    QPointF pos = (leftTop+rightBottom)/2;
    m_path.quadTo(m_point, pos);
    m_point = rightBottom;

    m_pItem->setPath(m_path);
}

void Pencil::CreateEnd(const QPointF &pos, GraphicsScene *pScene) {
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->Created();
}

void Pencil::SetSelected(bool selected) {
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}
void Pencil::SetEditable(bool editable) {

}
void Pencil::Remove(GraphicsScene *pScene) {
    pScene->removeItem(m_pItem);
}

void Pencil::RotateBegin() {

}
void Pencil::Rotate(qreal x, qreal y, qreal angle) {

}
void Pencil::RotateEnd() {

}

QRect Pencil::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF Pencil::GetP1() {
    return QPointF();
}
QPointF Pencil::GetP2() {
    return QPointF();
}
QPointF Pencil::GetPos() {
    return QPointF();
}
void Pencil::ChangePos(qreal dx, qreal dy) {

}
QGraphicsItem *Pencil::GetGraphicsItem() {
    return m_pItem;
}
void Pencil::ChangeSize(qreal dx, qreal dy) {

}

void Pencil::HideControls(bool hide) {
    m_pItem->HideClose(hide);
}

QPointF Pencil::MapFromScene(const QPointF &p){

    return p;
}

void Pencil::slotEraserMove(const QPointF &_prevPos, const QPointF &_pos, Eraser *eraser){
    /** 1 如果prevPos == pos 且 pos落在rcLayer之内（包含边界)，则说明有新的path要创建，起点在item内
     *  2 如果prevPos 在外面，而pos在item内，则说明有新的path要创建，且起点在直线(prevPos，pos)与item的交点pn，path.moveTo(pn),path.lineTo(pos)
     *  3 如果prevPos，pos都在item内部，则说明这个点是滑动过程中的点，将pos加入path
     *  4 如果prevPos在item内，而pos在item外部，说明鼠标已经滑出item，取直线(prevPos,pos)与item的交点pm,path.lineTo(pm)
     *  5 如果prevPos，pos横跨item区域，则取两个交点之间的直线作为path
     * */
    //qDebug() << "Pencil::slotEraserMove prevPos=" << _prevPos << ", pos" << _pos;

    bool bingo = false;

    QPointF posLayer = m_pItem->pos();
    QRectF rcLayer = m_pItem->boundingRect();
    //qDebug()<<"Pencil::slotEraserMove m_pItem.rect="<<rcLayer<<", pos="<<posLayer;
    //rcLayer = QRectF(rcLayer.x()-posLayer.x(), rcLayer.y()-posLayer.y(), rcLayer.width(), rcLayer.height());
    QPointF prevPos = _prevPos - posLayer;
    QPointF pos = _pos - posLayer;

    //四条边线
    qreal x = rcLayer.x();
    qreal y = rcLayer.y();
    qreal w = rcLayer.width();
    qreal h = rcLayer.height();
    QList<QLineF> edgeLines = {QLineF(x,y, x+w,y), QLineF(x+w,y, x+w,y+h), QLineF(x,y+h, x+w,y+h), QLineF(x,y, x,y+h)};

    //1 如果prevPos == pos 且 pos落在rcLayer之内（包含边界)，则说明有新的path要创建，起点在item内
    if(pos == prevPos && rcLayer.contains(prevPos.toPoint()) && rcLayer.contains(pos.toPoint()))
    {
        bingo = true;
        QPainterPath *path  = new QPainterPath();
        path->moveTo(MapFromScene(pos));
        ERASER_PATH *path_data = new ERASER_PATH(path, eraser->Width());

        long key = reinterpret_cast<long>(eraser);
        MapEraserPath::iterator it = m_pItem->m_mapEraserPathUndo.find(key);
        if(it != m_pItem->m_mapEraserPathUndo.end()){
            VectorPath *vectorPath = it.value();
            vectorPath->append(path_data);
        }else{
            VectorPath *vectorPath = new VectorPath();
            vectorPath->append(path_data);
            m_pItem->m_mapEraserPathUndo[key] = vectorPath;
        }
    }
    //如果prevPos 在外面，而pos在item内，则说明有新的path要创建，且起点在直线(prevPos，pos)与item的交点iPos，path.moveTo(iPos),path.lineTo(pos)
    //这种情况下只有一个交点
    else if(pos != prevPos && !rcLayer.contains(prevPos.toPoint()) && rcLayer.contains(pos.toPoint()))
    {
        bingo = true;
        //取直线(prevPos,pos)与item的交点
        QLineF line(prevPos, pos);
        bool found = false;
        QPointF iPos;//交点
        foreach(auto edgeLine, edgeLines){
            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
            if (intersectType == QLineF::BoundedIntersection){
                found = true;
                break;
            }
        }
        if(found){
            QPainterPath *path  = new QPainterPath();
            path->moveTo(MapFromScene(pos));
            ERASER_PATH *path_data = new ERASER_PATH(path, eraser->Width());
            //LOGER << "eraser moveTo "<<iPos.x()<<","<<iPos.y()<<EOL;
            path->moveTo(MapFromScene(iPos));
            long key = reinterpret_cast<long>(eraser);
            MapEraserPath::iterator it = m_pItem->m_mapEraserPathUndo.find(key);
            if(it != m_pItem->m_mapEraserPathUndo.end()){
                VectorPath *vectorPath = it.value();
                vectorPath->append(path_data);
            }else{
                VectorPath *vectorPath = new VectorPath();
                vectorPath->append(path_data);
                m_pItem->m_mapEraserPathUndo[key] = vectorPath;
            }
            //LOGER << "eraser lineTo "<<pos.x()<<","<<pos.y()<<EOL;
            path->lineTo(MapFromScene(pos));
        }
    }
    //3 如果prevPos，pos都在item内部，则说明这个点是滑动过程中的点，将pos加入path(最后一个）
    else if(pos != prevPos && rcLayer.contains(prevPos.toPoint()) && rcLayer.contains(pos.toPoint())){
        bingo = true;
        long key = reinterpret_cast<long>(eraser);
        MapEraserPath::iterator it = m_pItem->m_mapEraserPathUndo.find(key);
        if(it != m_pItem->m_mapEraserPathUndo.end()){
            VectorPath *vectorPath = it.value();
            QPainterPath *path = vectorPath->last()->path;
            path->lineTo(MapFromScene(pos));
        }
    }
    //4 如果prevPos在item内，而pos在item外部，说明鼠标已经滑出item，取直线(prevPos,pos)与item的交点iPos,path.lineTo(iPos)
    //这种情况下只有一个交点
    else if(pos != prevPos && rcLayer.contains(prevPos.toPoint()) && !rcLayer.contains(pos.toPoint())){
        bingo = true;
        //取直线(prevPos,pos)与item的交点
        QLineF line(prevPos, pos);
        bool found = false;
        QPointF iPos;//交点
        foreach(auto edgeLine, edgeLines){
            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
            if (intersectType == QLineF::BoundedIntersection){
                found = true;
                break;
            }
        }
        if(found){
            long key = reinterpret_cast<long>(eraser);
            MapEraserPath::iterator it = m_pItem->m_mapEraserPathUndo.find(key);
            if(it != m_pItem->m_mapEraserPathUndo.end()){
                VectorPath *vectorPath = it.value();
                QPainterPath *path = vectorPath->last()->path;
                path->lineTo(MapFromScene(iPos));
            }
        }
    }
    //5 如果prevPos，pos横跨item区域，则取两个交点之间的直线作为path(新创建path)
    //这种情况下，有一个或多个交点,如果 只有一个交点，则不需要处理；当鼠标轨迹和边框重合时，path为多个点。
    else if(pos != prevPos && !rcLayer.contains(prevPos.toPoint()) && !rcLayer.contains(pos.toPoint())){
        bingo = true;
        //取直线(prevPos,pos)与item的交点
        QLineF line(prevPos, pos);
        QList<QPointF> iPosList;//交点列表
        foreach(auto edgeLine, edgeLines){
            QPointF iPos;
            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
            if (intersectType == QLineF::BoundedIntersection){
                iPosList.append(iPos);
            }
        }
        if(iPosList.size() > 1){
            QPainterPath *path  = new QPainterPath();
            path->moveTo(MapFromScene(pos));
            ERASER_PATH *path_data = new ERASER_PATH(path, eraser->Width());
            long key = reinterpret_cast<long>(eraser);
            MapEraserPath::iterator it = m_pItem->m_mapEraserPathUndo.find(key);
            if(it != m_pItem->m_mapEraserPathUndo.end()){
                VectorPath *vectorPath = it.value();
                vectorPath->append(path_data);
            }else{
                VectorPath *vectorPath = new VectorPath();
                vectorPath->append(path_data);
                m_pItem->m_mapEraserPathUndo[key] = vectorPath;
            }

            QList<QPointF>::iterator itP = iPosList.begin();
            if(itP != iPosList.end()){
                path->moveTo(MapFromScene(*itP++));
            }
            while(itP != iPosList.end()){
                path->lineTo(MapFromScene(*itP++));
            }
        }
    }

    if(bingo){
        m_pItem->update();
    }
}

void Pencil::slotEraserPressed(){
    m_pItem->HideClose(true);
    m_pItem->setSelected(false);
}

void Pencil::slotEraserRelease(){
    m_pItem->onEraserRelease();
}
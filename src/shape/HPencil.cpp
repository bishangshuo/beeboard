#include "HPencil.h"
#include "src/graphics/GraphicsView.h"
//#include "board/property/HPropertyObject.h"
//#include "util/Loger.h"
//#include <QtConcurrent>
#include <QLineF>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

const qreal PI = 3.1415926;
const qreal ARC = PI/180;

inline QRect MakeSquare(const QRect &rect){

    if(!rect.isValid())
        return rect;

    int x, y, w, h;
    int width = rect.width();
    int height = rect.height();
    if(width > height){
        x = rect.x();
        y = rect.y() - (width-height)/2;
        w = width;
        h = w;
    }else{
        y = rect.y();
        x = rect.x() - (height-width)/2;
        h = height;
        w = h;
    }

    return QRect(x, y, w, h);
}

HPencil::HPencil(QObject *parent)
    : ShapeBase(parent)
    , m_pItem(NULL)
    , m_pOrgPixmap(NULL)
    , m_pPen(0)
    , m_pBrush(0)
{
    //m_nToolType = TOOL_TYPE::PENCIL;
    bCreated = false;
    m_rAngle = 0;
}

HPencil::~HPencil()
{



}

/*
qlonglong HPencil::LoadFromPixmap(const QPixmap &pixmap, const QRect &rect, GraphicsScene *scene){

    if(m_pItem)
    {
        return 0;
    }
    mPixmap = pixmap;
    m_rcLayer = rect;
    m_rcLayer = MakeSquare(m_rcLayer);
    bCreated = true;

    m_nBeginPos = QPointF(rect.x(),  rect.y());
    m_nEndPos = QPointF(rect.x()+rect.width(),  rect.y()+rect.height());

    m_nBeginPos = QPointF(rect.x(), rect.y());
    m_nEndPos = QPointF(rect.x()+rect.width(), rect.y()+rect.height());

    mPathCur.clear();
    mPathPre.clear();
    mPath.clear();
    mListPoint.clear();

    HScribbleItem *pItem = new HScribbleItem();
    qlonglong key = reinterpret_cast<qlonglong>(pItem);
    qDebug() << "item data key:"<<key;
    pItem->setData(ITEM_DATA_KEY, QVariant(key));
    pItem->setVisible(true);

    pItem->setRect(QRectF(0, 0, 1, 1));

    scene->addItem(pItem);

    m_pItem = pItem;
    mPixmap = pixmap;

    pItem->setRect(m_rcLayer);
    pItem->SetPixmap(mPixmap);
    pItem->update();

    SaveOrgPixmap(mPixmap);

    return key;
}
*/
int HPencil::Create(const QPointF &beginPos, const QPointF &endPos, GraphicsScene *scene)
{
    if(m_pItem)
    {
        return 0;
    }
    QSize desktopSize = qApp->desktop()->screenGeometry().size();
    m_pItem = new WbCanvasItem(desktopSize);
//    m_pItem->setX(-desktopSize.width()/4);
//    m_pItem->setY(0);
    //m_pItem->setBackgroundColor(Qt::transparent);
    qlonglong key = reinterpret_cast<qlonglong>(m_pItem);
    qDebug() << "item data key:"<<key;
    m_pItem->setData(ITEM_DATA_KEY, QVariant(key));
    m_pItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_pItem->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    m_pItem->setVisible(true);

    m_pItem->drawPress(key, beginPos);
    scene->addItem(m_pItem);

    return key;
}

void HPencil::UpdateRect(const QPointF &leftTop, const QPointF &rightBottom, GraphicsScene *pScene)
{
    Q_UNUSED(pScene)
    if(!m_pItem)
    {
        return;
    }
    qlonglong key = reinterpret_cast<qlonglong>(m_pItem);
    m_pItem->drawMove(key, leftTop, rightBottom);
}

void HPencil::CreateEnd(const QPointF &pos, GraphicsScene *pScene){
    qlonglong key = reinterpret_cast<qlonglong>(m_pItem);
    m_pItem->drawRelease(key, pos);
}

void HPencil::RenderPath(const QPainterPath &path, GraphicsScene *scene){
    HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);

    QRectF rc = path.boundingRect();
    QPixmap pixmap(scene->sceneRect().width(), scene->sceneRect().height());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QPen pen(pItem->pen());
    painter.setPen(pen);
    painter.drawPath(path);
    painter.end();

    int PENWIDTH = pItem->pen().width();
    int left = rc.x()-PENWIDTH/2-1;
    int top = rc.y()-PENWIDTH/2-1;
    int width = rc.width()+PENWIDTH+2;
    int height = rc.height()+PENWIDTH+2;

    if(left < 0) {
        width = width +left;
        left =  0;
    }
    if(top < 0) {
        height = height + top;
        top = 0;
    }

    m_rcLayer = QRect(left, top,  width,  height);
    //m_rcLayer = MakeSquare(m_rcLayer);
    mPixmap = pixmap.copy(m_rcLayer);

   // mPixmap = MixEraser(mPixmap);

    pItem->setRect(m_rcLayer);
    pItem->SetPixmap(mPixmap);
    pItem->update();
}

void HPencil::RenderPixmap(const QPixmap &pixmap, const QRect &rc, GraphicsScene *scene){
    HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);

    int PENWIDTH = pItem->pen().width();
    int left = rc.x()-PENWIDTH/2-1;
    int top = rc.y()-PENWIDTH/2-1;
    int width = rc.width()+PENWIDTH+2;
    int height = rc.height()+PENWIDTH+2;

    if(left < 0) {
        width = width +left;
        left =  0;
    }
    if(top < 0) {
        height = height + top;
        top = 0;
    }

    m_rcLayer = QRect(left, top,  width,  height);
    m_rcLayer = MakeSquare(m_rcLayer);
    mPixmap = pixmap.copy(m_rcLayer);
    pItem->setRect(m_rcLayer);
    pItem->SetPixmap(mPixmap);
    pItem->update();
}

void HPencil::SaveOrgPixmap(const QPixmap &pixmap){
    if(m_pOrgPixmap != NULL){
        delete m_pOrgPixmap;
        m_pOrgPixmap = NULL;
    }
    m_pOrgPixmap = new QPixmap(pixmap);
}

void HPencil::SetSelected(bool selected) {
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setSelected(selected);
}

void HPencil::SetEditable(bool editable) {
    if(m_pItem == nullptr){
        return;
    }
    m_pItem->setFlag(QGraphicsItem::ItemIsMovable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsSelectable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemIsFocusable, editable);
    m_pItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, editable);
    m_pItem->setAcceptHoverEvents(editable);
    m_pItem->setSelected(editable);
}

void HPencil::Remove(GraphicsScene *pScene) {
    pScene->removeItem(m_pItem);
}

void HPencil::RotateBegin() {

}

void HPencil::Rotate(qreal x, qreal y, qreal angle) {

}

void HPencil::RotateEnd() {
}

QRect HPencil::GetRect(){
    return m_pItem->sceneBoundingRect().toRect();
}

QPointF HPencil::GetP1(){
    return QPointF();//m_pItem->line().p1();
}

QPointF HPencil::GetP2(){
    return QPointF();//m_pItem->line().p2();
}

QPointF HPencil::GetPos(){
    return m_pItem->scenePos();
}

void HPencil::ChangePos(qreal dx, qreal dy){
    m_pItem->moveBy(dx, dy);
}

QGraphicsItem *HPencil::GetGraphicsItem(){
    return m_pItem;
}

void HPencil::ChangeSize(qreal dx, qreal dy){
//    QPointF p1 = m_pItem->line().p1();
//    QPointF p2 = m_pItem->line().p2();
//    QPointF p3 = QPointF(p2.x()+dx, p2.y()+dy);
//    m_pItem->setLine(p1.x(), p1.y(), p3.x(), p3.y());
}

void HPencil::HideControls(bool hide){
    //m_pItem->HideClose(hide);
    //m_pItem->HideResize(hide);
}

//void HPencil::slotEraserMove(const QPointF &prevPos, const QPointF &pos, HEraser *eraser){

//    /** 1 如果prevPos == pos 且 pos落在m_rcLayer之内（包含边界)，则说明有新的path要创建，起点在item内
//     *  2 如果prevPos 在外面，而pos在item内，则说明有新的path要创建，且起点在直线(prevPos，pos)与item的交点pn，path.moveTo(pn),path.lineTo(pos)
//     *  3 如果prevPos，pos都在item内部，则说明这个点是滑动过程中的点，将pos加入path
//     *  4 如果prevPos在item内，而pos在item外部，说明鼠标已经滑出item，取直线(prevPos,pos)与item的交点pm,path.lineTo(pm)
//     *  5 如果prevPos，pos横跨item区域，则取两个交点之间的直线作为path
//     * */

//    //四条边线
//    qreal x = m_rcLayer.x();
//    qreal y = m_rcLayer.y();
//    qreal w = m_rcLayer.width();
//    qreal h = m_rcLayer.height();
//    QList<QLineF> edgeLines = {QLineF(x,y, x+w,y), QLineF(x+w,y, x+w,y+h), QLineF(x,y+h, x+w,y+h), QLineF(x,y, x,y+h)};

//    //1 如果prevPos == pos 且 pos落在m_rcLayer之内（包含边界)，则说明有新的path要创建，起点在item内
//    if(pos == prevPos && m_rcLayer.contains(prevPos.toPoint()) && m_rcLayer.contains(pos.toPoint()))
//    {

//        QPainterPath *path  = new QPainterPath();

//        path->moveTo(MapFromScene(pos));

//        long key = reinterpret_cast<long>(eraser);
//        MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//        if(it != m_mapEraserPathUndo.end()){
//            VectorPath *vectorPath = it.value();
//            vectorPath->append(path);
//        }else{
//            VectorPath *vectorPath = new VectorPath();
//            vectorPath->append(path);
//            m_mapEraserPathUndo[key] = vectorPath;
//        }
//    }
//    //如果prevPos 在外面，而pos在item内，则说明有新的path要创建，且起点在直线(prevPos，pos)与item的交点iPos，path.moveTo(iPos),path.lineTo(pos)
//    //这种情况下只有一个交点
//    else if(pos != prevPos && !m_rcLayer.contains(prevPos.toPoint()) && m_rcLayer.contains(pos.toPoint()))
//    {
//        //取直线(prevPos,pos)与item的交点
//        QLineF line(prevPos, pos);
//        bool found = false;
//        QPointF iPos;//交点
//        foreach(auto edgeLine, edgeLines){
//            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
//            if (intersectType == QLineF::BoundedIntersection){
//                found = true;
//                break;
//            }
//        }
//        if(found){
//            QPainterPath *path  = new QPainterPath();
//            LOGER << "eraser moveTo "<<iPos.x()<<","<<iPos.y()<<EOL;
//            path->moveTo(MapFromScene(iPos));
//            long key = reinterpret_cast<long>(eraser);
//            MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//            if(it != m_mapEraserPathUndo.end()){
//                VectorPath *vectorPath = it.value();
//                vectorPath->append(path);
//            }else{
//                VectorPath *vectorPath = new VectorPath();
//                vectorPath->append(path);
//                m_mapEraserPathUndo[key] = vectorPath;
//            }
//            LOGER << "eraser lineTo "<<pos.x()<<","<<pos.y()<<EOL;
//            path->lineTo(MapFromScene(pos));
//        }
//    }
//    //3 如果prevPos，pos都在item内部，则说明这个点是滑动过程中的点，将pos加入path(最后一个）
//    else if(pos != prevPos && m_rcLayer.contains(prevPos.toPoint()) && m_rcLayer.contains(pos.toPoint())){
//        long key = reinterpret_cast<long>(eraser);
//        MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//        if(it != m_mapEraserPathUndo.end()){
//            VectorPath *vectorPath = it.value();
//            QPainterPath *path = vectorPath->last();
//            path->lineTo(MapFromScene(pos));
//        }
//    }
//    //4 如果prevPos在item内，而pos在item外部，说明鼠标已经滑出item，取直线(prevPos,pos)与item的交点iPos,path.lineTo(iPos)
//    //这种情况下只有一个交点
//    else if(pos != prevPos && m_rcLayer.contains(prevPos.toPoint()) && !m_rcLayer.contains(pos.toPoint())){
//        //取直线(prevPos,pos)与item的交点
//        QLineF line(prevPos, pos);
//        bool found = false;
//        QPointF iPos;//交点
//        foreach(auto edgeLine, edgeLines){
//            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
//            if (intersectType == QLineF::BoundedIntersection){
//                found = true;
//                break;
//            }
//        }
//        if(found){
//            long key = reinterpret_cast<long>(eraser);
//            MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//            if(it != m_mapEraserPathUndo.end()){
//                VectorPath *vectorPath = it.value();
//                QPainterPath *path = vectorPath->last();
//                path->lineTo(MapFromScene(iPos));
//            }
//        }
//    }
//    //5 如果prevPos，pos横跨item区域，则取两个交点之间的直线作为path(新创建path)
//    //这种情况下，有一个或多个交点,如果 只有一个交点，则不需要处理；当鼠标轨迹和边框重合时，path为多个点。
//    else if(pos != prevPos && !m_rcLayer.contains(prevPos.toPoint()) && !m_rcLayer.contains(pos.toPoint())){
//        //取直线(prevPos,pos)与item的交点
//        QLineF line(prevPos, pos);
//        QList<QPointF> iPosList;//交点列表
//        foreach(auto edgeLine, edgeLines){
//            QPointF iPos;
//            QLineF::IntersectType intersectType = line.intersects(edgeLine, &iPos);
//            if (intersectType == QLineF::BoundedIntersection){
//                iPosList.append(iPos);
//            }
//        }
//        if(iPosList.size() > 1){
//            QPainterPath *path  = new QPainterPath();
//            long key = reinterpret_cast<long>(eraser);
//            MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//            if(it != m_mapEraserPathUndo.end()){
//                VectorPath *vectorPath = it.value();
//                vectorPath->append(path);
//            }else{
//                VectorPath *vectorPath = new VectorPath();
//                vectorPath->append(path);
//                m_mapEraserPathUndo[key] = vectorPath;
//            }

//            QList<QPointF>::iterator itP = iPosList.begin();
//            if(itP != iPosList.end()){
//                path->moveTo(MapFromScene(*itP++));
//            }
//            while(itP != iPosList.end()){
//                path->lineTo(MapFromScene(*itP++));
//            }
//        }
//    }

//    //将所有path绘制在mPixmap上
//    mPixmap = MixEraser(mPixmap);
//}

//QPixmap HPencil::MixEraser(const QPixmap &pm){
//    if(pm.isNull())
//        return pm;
//    if(m_mapEraserPathUndo.size() > 0){
//        QPixmap pixmap = pm;
//        QPainter painter(&pixmap);
//        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
//        QPen pen;
//        int penWidth = HPropertyObject::getInstance()->EraserWidth();
//        pen.setWidth(penWidth);
//        pen.setCapStyle(Qt::RoundCap);
//        pen.setJoinStyle(Qt::RoundJoin);
//        pen.setColor(Qt::white);
//        painter.setPen(pen);
//        for(MapEraserPath::iterator it = m_mapEraserPathUndo.begin(); it != m_mapEraserPathUndo.end(); it ++){
//            VectorPath *vectorPath = it.value();
//            for(VectorPath::Iterator itV = vectorPath->begin(); itV != vectorPath->end(); itV++){
//                QPainterPath *path = *itV;
//                if(path->elementCount() > 1){
//                    painter.drawPath(*path);
//                }
//            }
//        }
//        painter.end();

//        HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);
//        pItem->SetPixmap(pixmap);
//        pItem->update();
//        return pixmap;
//    }
//    return pm;
//}

//void HPencil::slotEraserRelease(const QPointF &pos, HEraser *eraser){
//    long key = reinterpret_cast<long>(eraser);
//    MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//    if(it != m_mapEraserPathUndo.end()){
//        qlonglong key = reinterpret_cast<qlonglong>(m_pItem);
//        emit sigEraseComplete(key);

//        QPixmap *pixmap = new QPixmap();
//        *pixmap = mPixmap.copy();
//        QtConcurrent::run([this, key, pixmap](){
//            QImage img = pixmap->toImage();
//            QRgb *st = (QRgb *) img.bits();
//            quint64 pixelCount = img.width() * img.height();

//            bool b = false;
//            for (quint64 p = 0; p < pixelCount; p++) {
//                //st[p] = QColor(255, 128, 0).rgb();
//                QRgb rgb = st[p];
//                int alpha = qAlpha(rgb);
//                if(alpha > 16){
//                    b = true;
//                    break;
//                }
//            }
//            if(!b){
//                this->sigShouldRemovePencil(key);
//            }
//        });
//    }

//    mPixmap = MixEraser(mPixmap);
//    emit sigEraseComplete(key);
//}

QPointF HPencil::MapFromScene(const QPointF &p){
    qreal x1 = p.x() - m_rcLayer.x();
    qreal y1 = p.y() - m_rcLayer.y();

    /*
     *  x'=t+(x-t)cosα-(y-r)sinα
     *  y'=r+(x-t)sinα+(y-r)cosα
    */

    qreal x0 = m_rcLayer.center().x() - m_rcLayer.x();
    qreal y0 = m_rcLayer.center().y() - m_rcLayer.y();

    m_rAngle = m_pItem->rotation();
    qreal pAngle = -m_rAngle;
    qreal x2 = x0+(x1-x0)*cos(pAngle*ARC) - (y1-y0)*sin(pAngle*ARC);
    qreal y2 = y0+(x1-x0)*sin(pAngle*ARC) + (y1-y0)*cos(pAngle*ARC);

    return QPointF(x2, y2);
//    return QPointF(x1, y1);
}

qreal HPencil::TrimAngle(qreal angle){
    qreal temp = angle;
    if (temp > 360.0)
    {
        while(1)
        {
            temp -= 360.0;
            if (temp < 360.0) break;
        }
    }
    if (temp < 0.0)
    {
        while(1)
        {
            temp += 360.0;
            if (temp > 0.0) break;
        }
    }
    return temp;
}

//void HPencil::Move(int dx, int dy, GraphicsScene *scene){
//    HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);
//    if(pItem != NULL){
//        QPointF pt = pItem->pos();
//        pItem->setPos(pt.x()+dx, pt.y()+dy);
//        int x = m_rcLayer.x();
//        int y = m_rcLayer.y();
//        int w = m_rcLayer.width();
//        int h = m_rcLayer.height();
//        m_rcLayer = QRect(x+dx, y+dy, w, h);
//        m_rcLayer = MakeSquare(m_rcLayer);
//    }
//}

//void HPencil::slotUndoEraser(HEraser *eraser){
//    bool b = false;
//    long key = reinterpret_cast<long>(eraser);

//    //重做擦除
//    MapEraserPath::iterator it = m_mapEraserPathUndo.find(key);
//    if(it != m_mapEraserPathUndo.end()){
//        VectorPath *vectorPath = it.value();
//        //不要删除
//        //delete vectorPath;
//        it = m_mapEraserPathUndo.erase(it);
//        b = true;

//        //加入redo
//        m_mapEraserPathRedo[key] = vectorPath;
//    }
//    if(b){
//        //将橡皮擦路径与原始地图合成, 赋值给绘制对象mPixmap
//        mPixmap = MixEraser(*m_pOrgPixmap);
//        HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);
//        pItem->SetPixmap(mPixmap);
//        pItem->update();
//        emit sigEraseComplete(key);
//    }

//}

//void HPencil::slotRedoEraser(HEraser *eraser){
//    bool b = false;
//    long key = reinterpret_cast<long>(eraser);
//    MapEraserPath::iterator it = m_mapEraserPathRedo.find(key);
//    if(it != m_mapEraserPathRedo.end()){
//        VectorPath *vectorPath = it.value();
//        //不要删除
//        //delete vectorPath;
//        it = m_mapEraserPathRedo.erase(it);
//        b = true;

//        //加入redo
//        m_mapEraserPathUndo[key] = vectorPath;
//    }
//    if(b){
//        //将橡皮擦路径与原始地图合成, 赋值给绘制对象mPixmap
//        mPixmap = MixEraser(*m_pOrgPixmap);
//        HScribbleItem *pItem = dynamic_cast<HScribbleItem *>(m_pItem);
//        pItem->SetPixmap(mPixmap);
//        pItem->update();
//        emit sigEraseComplete(key);
//    }
//}

//void HPencil::slotClearRedo(){
//    HShape::slotClearRedo();

//    for(MapEraserPath::iterator it = m_mapEraserPathRedo.begin(); it != m_mapEraserPathRedo.end(); ){
//        VectorPath *vectorPath = it.value();
//        for(VectorPath::iterator itV = vectorPath->begin(); itV != vectorPath->end(); ){
//            QPainterPath *path = *itV;
//            delete path;
//            itV = vectorPath->erase(itV);
//        }
//        delete  vectorPath;
//        it = m_mapEraserPathRedo.erase(it);
//    }
//    m_mapEraserPathRedo.clear();
//}

//void HPencil::Scale(qreal percent, GraphicsScene *scene){

//}

#include "hscribbleitem.h"
#include <QGraphicsScene>
#include <QColor>
#include <QDebug>
//#include "util/Loger.h"
#include <QGraphicsSceneHoverEvent>
//#include "board/shape/HEraser.h"
//#include "board/property/HPropertyObject.h"

HScribbleItem::HScribbleItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsSelectable);
//    mouseTrack
}

HScribbleItem::~HScribbleItem(){
}

void HScribbleItem::SetPixmap(const QPixmap &pixmap){
    mPixmap = pixmap;
}

void HScribbleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF rc = rect();

    painter->drawPixmap(rc, mPixmap, QRectF(0, 0, rc.width(), rc.height()));
}

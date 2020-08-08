#include "PencilImageItem.h"
#include <QPainter>

PencilImageItem::PencilImageItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
{

}

void PencilImageItem::setPixmap(const QPixmap &pixmap){
    m_pixmap = pixmap;
    update();
}

void PencilImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsRectItem::paint(painter, option, widget);
    if(!m_pixmap.isNull()){
        QRectF rc = rect();
        painter->drawPixmap(rc, m_pixmap, QRectF(0, 0, rc.width(), rc.height()));
    }
}

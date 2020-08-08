#ifndef PENCILIMAGEITEM_H
#define PENCILIMAGEITEM_H

#include <QGraphicsRectItem>

class PencilImageItem : public QGraphicsRectItem
{
public:
    PencilImageItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    void setPixmap(const QPixmap &pixmap);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QPixmap m_pixmap;
};

#endif // PENCILIMAGEITEM_H

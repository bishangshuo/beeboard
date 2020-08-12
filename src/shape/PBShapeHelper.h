#ifndef PBSHAPEHELPER_H
#define PBSHAPEHELPER_H

#include <QObject>
#include <QPen>

#include "pbshape.pb.h"
#include "Pencil.h"

class PBShapeHelper : public QObject
{
    Q_OBJECT
public:
    explicit PBShapeHelper(QObject *parent = nullptr);

signals:
public:
    static PBShape::Line        *CreateLine(const QPoint &p0, const QPoint &p1, const QPen &p);
    static PBShape::Rectangle   *CreateRectangle(int x, int y, int width, int height, const QPen &p, const QBrush &b);
    static PBShape::Ellipse     *CreateEllipse(int x, int y, int width, int height, const QPen &p, const QBrush &b);
    static PBShape::Triangle    *CreateTriangle(int x, int y, int width, int height, const QPen &p, const QBrush &b);
    static PBShape::Scribble    *CreateScribble(int x, int y, int width, int height, const QPointF &pos, const QPen &p,
                                                const QByteArray &path_ba, const QList<ERASER_DATA> &eraser_data);
};

#endif // PBSHAPEHELPER_H

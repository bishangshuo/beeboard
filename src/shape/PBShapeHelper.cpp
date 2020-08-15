#include "PBShapeHelper.h"
#include "pbshape.pb.h"

PBShapeHelper::PBShapeHelper(QObject *parent) : QObject(parent)
{

}

PBShape::Line *PBShapeHelper::CreateLine(int x, int y, const QPoint &p0, const QPoint &p1, const QPen &p){
    PBShape::Line *line = new PBShape::Line;
    PBShape::Pos *_pos = new PBShape::Pos();
    _pos->set_x(x);
    _pos->set_y(y);
    PBShape::Pos *_p0 = new PBShape::Pos();
    _p0->set_x(p0.x());
    _p0->set_y(p0.y());
    PBShape::Pos *_p1 = new PBShape::Pos();
    _p1->set_x(p1.x());
    _p1->set_y(p1.y());
    line->set_allocated_pos(_pos);
    line->set_allocated_p0(_p0);
    line->set_allocated_p1(_p1);
    PBShape::Pen *pen = new PBShape::Pen();
    QColor penColor = p.color();
    int penWidth = p.width();
    pen->set_width(penWidth);
    pen->set_r(penColor.red());
    pen->set_g(penColor.green());
    pen->set_b(penColor.blue());
    pen->set_a(penColor.alpha());
    pen->set_style(Qt::SolidPattern);
    pen->set_capstyle(Qt::RoundCap);
    pen->set_joinstyle(Qt::RoundJoin);
    line->set_allocated_pen(pen);
    return line;
}

PBShape::Rectangle *PBShapeHelper::CreateRectangle(int x, int y, int width, int height, const QPen &p, const QBrush &b){
    PBShape::Rectangle *rectangle = new PBShape::Rectangle;
    PBShape::Pos *pos = new PBShape::Pos();
    pos->set_x(x);
    pos->set_y(y);
    rectangle->set_allocated_pos(pos);
    rectangle->set_width(width);
    rectangle->set_height(height);
    PBShape::Pen *pen = new PBShape::Pen();
    QColor penColor = p.color();
    int penWidth = p.width();
    pen->set_width(penWidth);
    pen->set_r(penColor.red());
    pen->set_g(penColor.green());
    pen->set_b(penColor.blue());
    pen->set_a(penColor.alpha());
    pen->set_style(Qt::SolidPattern);
    pen->set_capstyle(Qt::RoundCap);
    pen->set_joinstyle(Qt::RoundJoin);
    rectangle->set_allocated_pen(pen);
    PBShape::Brush *brush = new PBShape::Brush;
    QColor brushColor = b.color();
    brush->set_r(brushColor.red());
    brush->set_g(brushColor.green());
    brush->set_b(brushColor.blue());
    brush->set_a(brushColor.alpha());
    rectangle->set_allocated_brush(brush);
    return rectangle;
}

PBShape::Ellipse *PBShapeHelper::CreateEllipse(int x, int y, int width, int height, const QPen &p, const QBrush &b){
    PBShape::Ellipse *ellipse = new PBShape::Ellipse;
    PBShape::Pos *pos = new PBShape::Pos();
    pos->set_x(x);
    pos->set_y(y);
    ellipse->set_allocated_pos(pos);
    ellipse->set_width(width);
    ellipse->set_height(height);
    PBShape::Pen *pen = new PBShape::Pen();
    QColor penColor = p.color();
    int penWidth = p.width();
    pen->set_width(penWidth);
    pen->set_r(penColor.red());
    pen->set_g(penColor.green());
    pen->set_b(penColor.blue());
    pen->set_a(penColor.alpha());
    pen->set_style(Qt::SolidPattern);
    pen->set_capstyle(Qt::RoundCap);
    pen->set_joinstyle(Qt::RoundJoin);
    ellipse->set_allocated_pen(pen);
    PBShape::Brush *brush = new PBShape::Brush;
    QColor brushColor = b.color();
    brush->set_r(brushColor.red());
    brush->set_g(brushColor.green());
    brush->set_b(brushColor.blue());
    brush->set_a(brushColor.alpha());
    ellipse->set_allocated_brush(brush);
    return ellipse;
}

PBShape::Triangle *PBShapeHelper::CreateTriangle(int x, int y, int width, int height, const QPen &p, const QBrush &b){
    PBShape::Triangle *triangle = new PBShape::Triangle;
    PBShape::Pos *pos = new PBShape::Pos();
    pos->set_x(x);
    pos->set_y(y);
    triangle->set_allocated_pos(pos);
    triangle->set_width(width);
    triangle->set_height(height);
    PBShape::Pen *pen = new PBShape::Pen();
    QColor penColor = p.color();
    int penWidth = p.width();
    pen->set_width(penWidth);
    pen->set_r(penColor.red());
    pen->set_g(penColor.green());
    pen->set_b(penColor.blue());
    pen->set_a(penColor.alpha());
    pen->set_style(Qt::SolidPattern);
    pen->set_capstyle(Qt::RoundCap);
    pen->set_joinstyle(Qt::RoundJoin);
    triangle->set_allocated_pen(pen);
    PBShape::Brush *brush = new PBShape::Brush;
    QColor brushColor = b.color();
    brush->set_r(brushColor.red());
    brush->set_g(brushColor.green());
    brush->set_b(brushColor.blue());
    brush->set_a(brushColor.alpha());
    triangle->set_allocated_brush(brush);
    return triangle;
}

PBShape::Scribble *PBShapeHelper::CreateScribble(int x, int y, int width, int height, const QPointF &pos, const QPen &p,
                                                 const QByteArray &path_ba, const QList<ERASER_DATA> &eraser_data){
    PBShape::Scribble *scribble = new PBShape::Scribble;
    PBShape::Rect *rect = new PBShape::Rect;
    rect->set_x(x);
    rect->set_y(y);
    rect->set_w(width);
    rect->set_h(height);
    scribble->set_allocated_rect(rect);
    PBShape::Pen *pen = new PBShape::Pen();
    QColor penColor = p.color();
    int penWidth = p.width();
    pen->set_width(penWidth);
    pen->set_r(penColor.red());
    pen->set_g(penColor.green());
    pen->set_b(penColor.blue());
    pen->set_a(penColor.alpha());
    pen->set_style(Qt::SolidPattern);
    pen->set_capstyle(Qt::RoundCap);
    pen->set_joinstyle(Qt::RoundJoin);
    scribble->set_allocated_pen(pen);
    PBShape::Pos *pos_p = new PBShape::Pos();
    pos_p->set_x(pos.x());
    pos_p->set_y(pos.y());
    scribble->set_allocated_pos(pos_p);
    std::string *path_data = new std::string(path_ba.toStdString());
    scribble->set_allocated_path(path_data);
    int i = 1;
    for(QList<ERASER_DATA>::const_iterator it = eraser_data.constBegin(); it != eraser_data.constEnd(); it++){
        ERASER_DATA edata = *it;
        PBShape::Eraser *eraser = new PBShape::Eraser();
        eraser->set_width(edata.width);
        QByteArray *eba = edata.ba;
        eraser->set_allocated_path(new std::string(eba->toStdString()));
        (*scribble->mutable_maperaser())[i++] = *eraser;
    }
    return scribble;
}

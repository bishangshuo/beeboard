#include "PBShapeHelper.h"
#include "pbshape.pb.h"

PBShapeHelper::PBShapeHelper(QObject *parent) : QObject(parent)
{

}

PBShape::Line *PBShapeHelper::CreateLine(const QPoint &p0, const QPoint &p1, const QPen &p){
    PBShape::Line *line = new PBShape::Line;
    PBShape::Pos *_p0 = new PBShape::Pos();
    _p0->set_x(p0.x());
    _p0->set_y(p0.y());
    PBShape::Pos *_p1 = new PBShape::Pos();
    _p1->set_x(p1.x());
    _p1->set_y(p1.y());
    line->set_allocated_p0(_p0);
    line->set_allocated_p1(_p0);
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
    PBShape::Rect *rect = new PBShape::Rect;
    rect->set_x(x);
    rect->set_y(y);
    rect->set_w(width);
    rect->set_h(height);
    rectangle->set_allocated_rect(rect);
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
    PBShape::Rect *rect = new PBShape::Rect;
    rect->set_x(x);
    rect->set_y(y);
    rect->set_w(width);
    rect->set_h(height);
    ellipse->set_allocated_rect(rect);
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
    PBShape::Rect *rect = new PBShape::Rect;
    rect->set_x(x);
    rect->set_y(y);
    rect->set_w(width);
    rect->set_h(height);
    triangle->set_allocated_rect(rect);
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

PBShape::Scribble *PBShapeHelper::CreateScribble(int x, int y, int width, int height, const QByteArray &ba){
    PBShape::Scribble *scribble = new PBShape::Scribble;
    PBShape::Rect *rect = new PBShape::Rect;
    rect->set_x(x);
    rect->set_y(y);
    rect->set_w(width);
    rect->set_h(height);
    scribble->set_allocated_rect(rect);
    std::string *pixmap_data = new std::string(ba.toStdString());
    scribble->set_allocated_pixmap(pixmap_data);
    return scribble;
}

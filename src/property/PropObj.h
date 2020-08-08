#ifndef PROPOBJ_H
#define PROPOBJ_H

#include <QObject>

class PropObj : public QObject
{
    Q_OBJECT
private:
    explicit PropObj(QObject *parent = nullptr);
public:
    static PropObj *GetInstance();

    int PenWidth() const;
    QColor PenColor() const;
    Qt::PenStyle PenStyle() const;
    Qt::PenCapStyle PenCapStyle() const;
    Qt::PenJoinStyle PenJoinStyle() const;
    QColor BrushColor() const;
signals:
private:
    static PropObj *g_pProp;
};

#endif // PROPOBJ_H

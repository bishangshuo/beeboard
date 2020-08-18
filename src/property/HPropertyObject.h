#ifndef HPROPERTYOBJECT_H
#define HPROPERTYOBJECT_H

#include <QObject>
#include <QtGui>

class HPropertyObject : public QObject
{
    Q_OBJECT
public:
    static HPropertyObject *getInstance();

    QColor PenColor() const;
    QColor BrushColor() const;
    int PenWidth() const;
    int EraserWidth() const;
    QColor BackgroundColor() const;

    int PenWidthIndex() const;
    int EraserWidthIndex() const;
    int PenColorIndex() const;
    int BrushColorIndex() const;
    int BackgroundColorIndex() const;

public slots:
    void procPenColorChanged(const int &index, const QColor &color);
    void procBrushColorChanged(const int &index, const QColor &color);
    void procPenWidthChanged(const int &index, const int &width);
    void procEraserWidthChanged(const int &index, const int &width);
    void procBackgroundColorChanged(const int &index, const QColor &color);
private:
    HPropertyObject(QObject *parent = nullptr);
private:
    QColor m_nPenColor;
    QColor m_nBrushColor;
    QColor m_nBackgroundColor;
    int m_nPenWidth;
    int m_nEraserWidth;

    int m_nPenColorIndex;
    int m_nBrushColorIndex;
    int m_nBackgroundColorIndex;
    int m_nPenWidthIndex;
    int m_nEraserWidthIndex;
};

#endif // HPROPERTYOBJECT_H

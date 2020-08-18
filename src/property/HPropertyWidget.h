#ifndef PROPERTY_H
#define PROPERTY_H

#include "component/hwidget.h"
#include <QMap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "component/HPushButton.h"
#include "component/HLabel.h"
#include "HPropertyObject.h"

class HPushButton;
class HPropertyWidget : public HWidget
{
    Q_OBJECT
public:
    explicit HPropertyWidget(HWidget *parent = NULL);
    ~HPropertyWidget();

signals:
    void sigPenColorChanged(const int &index, const QColor &color);
    void sigPenWidthChanged(const int &index, const int &width);
    void sigBrushColorChanged(const int &index, const QColor &color);
    void sigEraserWidthChanged(const int &index, const int &width);
private:
    void SetupView();
    void OnPropertyChanged(int type, int value);
    void SetDefaultValue();

protected:
    void showEvent(QShowEvent *event);

signals:
    void closeed();

private slots:
    void onCloseButtonClicked();
private:
    QMap<int, HPushButton *> m_mapPenColorButton;
    QMap<int, HPushButton *> m_mapBrushColorButton;
    QMap<int, HPushButton *> m_mapPenWidthButton;
    QMap<int, HPushButton *> m_mapEraserWidthButton;
};

#endif // PROPERTY_H

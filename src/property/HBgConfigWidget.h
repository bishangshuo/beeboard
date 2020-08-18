#ifndef HBGCONFIGWIDGET_H
#define HBGCONFIGWIDGET_H

#include <QWidget>
#include <QMap>

class HPushButton;

class HBgConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HBgConfigWidget(QWidget *parent = nullptr);

signals:
    void sigBackgroundColorChanged(const int &index, const QColor &color);
private:
    void SetupView();
    void OnBgSelected(int index);
    void SetDefaultValue();
private:
    QMap<int, HPushButton *> m_mapBtn;
};

#endif // HBGCONFIGWIDGET_H

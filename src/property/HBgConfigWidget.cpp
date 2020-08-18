#include "HBgConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "component/HPushButton.h"
#include "component/HLabel.h"
#include "HPropertyObject.h"

inline QColor BackgroundColorFromIndex(int index){
    switch(index){
    case 1:
        return QColor(36,66,66);
    case 2:
        return QColor(64,64,64);
    case 3:
        return QColor(210,210,210);
    case 4:
        return QColor(151,195,193);
    }
    return QColor(36,66,66);
}

HBgConfigWidget::HBgConfigWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(1);
    //setAttribute(Qt::WA_TranslucentBackground);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(0, 0, 0, 120));
    setAutoFillBackground(true);
    setPalette(pal);

    SetupView();
    connect(this,
            SIGNAL(sigBackgroundColorChanged(const int &, const QColor &)),
            HPropertyObject::getInstance(),
            SLOT(procBackgroundColorChanged(const int &, const QColor &)));
}

void HBgConfigWidget::SetupView()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setMargin(20);

    HPushButton *btnBg1 = new HPushButton();
    btnBg1->setIcon(QIcon(":/images/bg_1.png"));
    btnBg1->setIconSize(QSize(96, 96));
    mainLayout->addWidget(btnBg1);
    m_mapBtn[1] = btnBg1;
    QObject::connect(btnBg1, &QPushButton::clicked, [=]{
        OnBgSelected(1);
    });

    HPushButton *btnBg2 = new HPushButton();
    btnBg2->setIcon(QIcon(":/images/bg_2.png"));
    btnBg2->setIconSize(QSize(96, 96));
    mainLayout->addWidget(btnBg2);
    m_mapBtn[2] = btnBg2;
    QObject::connect(btnBg2, &QPushButton::clicked, [=]{
        OnBgSelected(2);
    });

    HPushButton *btnBg3 = new HPushButton();
    btnBg3->setIcon(QIcon(":/images/bg_3.png"));
    btnBg3->setIconSize(QSize(96, 96));
    mainLayout->addWidget(btnBg3);
    m_mapBtn[3] = btnBg3;
    QObject::connect(btnBg3, &QPushButton::clicked, [=]{
        OnBgSelected(3);
    });

    HPushButton *btnBg4 = new HPushButton();
    btnBg4->setIcon(QIcon(":/images/bg_4.png"));
    btnBg4->setIconSize(QSize(96, 96));
    mainLayout->addWidget(btnBg4);
    m_mapBtn[4] = btnBg4;
    QObject::connect(btnBg4, &QPushButton::clicked, [=]{
        OnBgSelected(4);
    });
}

void HBgConfigWidget::OnBgSelected(int index){
    QColor color = BackgroundColorFromIndex(index);
    for(QMap<int, HPushButton *>::iterator it = m_mapBtn.begin();
        it != m_mapBtn.end(); it ++){
        if(it.key() == index){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
    emit sigBackgroundColorChanged(index, color);
}

void HBgConfigWidget::SetDefaultValue()
{
    int colorIndex = HPropertyObject::getInstance()->BackgroundColorIndex();
    for(QMap<int, HPushButton *>::iterator it = m_mapBtn.find(colorIndex);
        it != m_mapBtn.end(); it ++){
        if(it.key() == colorIndex){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
}

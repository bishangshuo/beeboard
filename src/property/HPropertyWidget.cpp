#include "HPropertyWidget.h"


inline QColor PenColorFromIndex(int index){
    switch(index){
    case 1:
        return Qt::red;
    case 2:
        return Qt::yellow;
    case 3:
        return Qt::blue;
    case 4:
        return Qt::green;
    case 5:
        return Qt::white;
    case 6:
        return Qt::black;
    }
    return Qt::red;
}

inline QColor BrushColorFromIndex(int index){
    switch(index){
    case 0:
        return Qt::transparent;
    case 1:
        return Qt::red;
    case 2:
        return Qt::yellow;
    case 3:
        return Qt::blue;
    case 4:
        return Qt::green;
    case 5:
        return Qt::white;
    case 6:
        return Qt::black;
    }
    return Qt::red;
}

inline int PenWidthFromIndex(int index){
    switch(index){
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 6;
    case 4:
        return 8;
    case 5:
        return 10;
    case 6:
        return 12;
    }
    return Qt::red;
}

inline int EraserWidthFromIndex(int index){
    switch(index){
    case 1:
        return 10;
    case 2:
        return 20;
    case 3:
        return 40;
    case 4:
        return 60;
    case 5:
        return 100;
    case 6:
        return 140;
    }
    return Qt::red;
}

HPropertyWidget::HPropertyWidget(HWidget *parent) : HWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground,true);
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
            SIGNAL(sigPenColorChanged(const int &, const QColor &)),
            HPropertyObject::getInstance(),
            SLOT(procPenColorChanged(const int &, const QColor &)));
    connect(this,
            SIGNAL(sigBrushColorChanged(const int &, const QColor &)),
            HPropertyObject::getInstance(),
            SLOT(procBrushColorChanged(const int &, const QColor &)));
    connect(this,
            SIGNAL(sigPenWidthChanged(const int &, const int &)),
            HPropertyObject::getInstance(),
            SLOT(procPenWidthChanged(const int &, const int &)));
    connect(this,
            SIGNAL(sigEraserWidthChanged(const int &, const int &)),
            HPropertyObject::getInstance(),
            SLOT(procEraserWidthChanged(const int &, const int &)));

    SetDefaultValue();
}

HPropertyWidget::~HPropertyWidget()
{

}

void HPropertyWidget::SetupView()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setMargin(20);

    QHBoxLayout *headLayout = new QHBoxLayout();
    mainLayout->addLayout(headLayout);
    mainLayout->setAlignment(headLayout, Qt::AlignTop);
    mainLayout->setStretchFactor(headLayout, 1);
    HLabel *headTitle = new HLabel();
    headTitle->setText(tr("propertypicker"));
    headLayout->addWidget(headTitle);
    headLayout->setAlignment(headTitle, Qt::AlignLeft);
    HPushButton *closeButton = new HPushButton(false);
    closeButton->setIcon(QIcon(":/images/icon_close.png"));
    closeButton->setIconSize(QSize(20, 20));
    headLayout->addWidget(closeButton);
    headLayout->setAlignment(closeButton, Qt::AlignRight);
    connect(closeButton, &HPushButton::clicked, [=]{
        closeButton->setChecked(false);
        hide();
        emit closeed();
    });

    //分割线
    QHBoxLayout *spLayout = new QHBoxLayout();
    mainLayout->addLayout(spLayout);
    mainLayout->setStretchFactor(spLayout, 1);
//    QLabel *lineLabel = new QLabel();
//    spLayout->addWidget(lineLabel);
//    lineLabel->setPalette(QPalette(Qt::red));
//    lineLabel->setGeometry(0, 1, size().width(), 1);

    //画笔颜色
    QHBoxLayout *penLayout = new QHBoxLayout();
    mainLayout->addLayout(penLayout);
    mainLayout->setStretchFactor(penLayout, 2);
    mainLayout->setAlignment(penLayout, Qt::AlignTop);
    //-------------------------
    HLabel *penTitle = new HLabel();
    penTitle->setText(tr("linecolor"));
    penLayout->addWidget(penTitle);
    penLayout->setAlignment(penTitle, Qt::AlignLeft);
    //--------------------------
    QHBoxLayout *penColorLayout =  new QHBoxLayout();
    penLayout->addLayout(penColorLayout);
    penLayout->setAlignment(penColorLayout, Qt::AlignRight);
    HPushButton *redButton = new HPushButton();
    redButton->setIcon(QIcon(":/images/icon_color_red.png"));
    redButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(redButton);
    redButton->setChecked(true);
    m_mapPenColorButton[1] = redButton;
    QObject::connect(redButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 1);
    });
    HPushButton *yellowButton = new HPushButton();
    yellowButton->setIcon(QIcon(":/images/icon_color_yellow.png"));
    yellowButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(yellowButton);
    m_mapPenColorButton[2] = yellowButton;
    QObject::connect(yellowButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 2);
    });
    HPushButton *blueButton = new HPushButton();
    blueButton->setIcon(QIcon(":/images/icon_color_blue.png"));
    blueButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(blueButton);
    m_mapPenColorButton[3] = blueButton;
    QObject::connect(blueButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 3);
    });
    HPushButton *greenButton = new HPushButton();
    greenButton->setIcon(QIcon(":/images/icon_color_green.png"));
    greenButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(greenButton);
    m_mapPenColorButton[4] = greenButton;
    QObject::connect(greenButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 4);
    });
    HPushButton *whiteButton = new HPushButton();
    whiteButton->setIcon(QIcon(":/images/icon_color_white.png"));
    whiteButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(whiteButton);
    m_mapPenColorButton[5] = whiteButton;
    QObject::connect(whiteButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 5);
    });
    HPushButton *blackButton = new HPushButton();
    blackButton->setIcon(QIcon(":/images/icon_color_black.png"));
    blackButton->setIconSize(QSize(32, 32));
    penColorLayout->addWidget(blackButton);
    m_mapPenColorButton[6] = blackButton;
    QObject::connect(blackButton, &QPushButton::clicked, [=]{
        OnPropertyChanged(1, 6);
    });

    //画笔粗细
    QHBoxLayout *boldLayout = new QHBoxLayout();
    mainLayout->addLayout(boldLayout);
    mainLayout->setStretchFactor(boldLayout, 2);
    mainLayout->setAlignment(boldLayout, Qt::AlignTop);
    //-------------------------
    HLabel *boldTitle = new HLabel();
    boldTitle->setText(tr("penwidth"));
    boldLayout->addWidget(boldTitle);
    boldLayout->setAlignment(boldTitle, Qt::AlignLeft);
    //--------------------------
    QHBoxLayout *penWidthLayout =  new QHBoxLayout();
    boldLayout->addLayout(penWidthLayout);
    boldLayout->setAlignment(penWidthLayout, Qt::AlignRight);
    HPushButton *width2Button = new HPushButton();
    width2Button->setIcon(QIcon(":/images/icon_pen_width_2.png"));
    width2Button->setIconSize(QSize(32, 32));
    penWidthLayout->addWidget(width2Button);
    m_mapPenWidthButton[1] = width2Button;
    QObject::connect(width2Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 1);
    });
    HPushButton *width4Button = new HPushButton();
    width4Button->setIcon(QIcon(":/images/icon_pen_width_4.png"));
    width4Button->setIconSize(QSize(32, 32));
    width4Button->setChecked(true);
    penWidthLayout->addWidget(width4Button);
    m_mapPenWidthButton[2] = width4Button;
    QObject::connect(width4Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 2);
    });
    HPushButton *width6Button = new HPushButton();
    width6Button->setIcon(QIcon(":/images/icon_pen_width_6.png"));
    width6Button->setIconSize(QSize(32, 32));
    penWidthLayout->addWidget(width6Button);
    m_mapPenWidthButton[3] = width6Button;
    QObject::connect(width6Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 3);
    });
    HPushButton *width8Button = new HPushButton();
    width8Button->setIcon(QIcon(":/images/icon_pen_width_8.png"));
    width8Button->setIconSize(QSize(32, 32));
    penWidthLayout->addWidget(width8Button);
    m_mapPenWidthButton[4] = width8Button;
    QObject::connect(width8Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 4);
    });
    HPushButton *width10Button = new HPushButton();
    width10Button->setIcon(QIcon(":/images/icon_pen_width_10.png"));
    width10Button->setIconSize(QSize(32, 32));
    penWidthLayout->addWidget(width10Button);
    m_mapPenWidthButton[5] = width10Button;
    QObject::connect(width10Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 5);
    });
    HPushButton *width12Button = new HPushButton();
    width12Button->setIcon(QIcon(":/images/icon_pen_width_12.png"));
    width12Button->setIconSize(QSize(32, 32));
    penWidthLayout->addWidget(width12Button);
    m_mapPenWidthButton[6] = width12Button;
    QObject::connect(width12Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(2, 6);
    });

    //填充颜色
    QHBoxLayout *fillLayout = new QHBoxLayout();
    mainLayout->addLayout(fillLayout);
    mainLayout->setStretchFactor(fillLayout, 2);
    mainLayout->setAlignment(fillLayout, Qt::AlignTop);
    //-------------------------
    HLabel *fillTitle = new HLabel();
    fillTitle->setText(tr("fillcolor"));
    fillLayout->addWidget(fillTitle);
    fillLayout->setAlignment(fillTitle, Qt::AlignLeft);
    //--------------------------
    QHBoxLayout *fillColorLayout =  new QHBoxLayout();
    fillLayout->addLayout(fillColorLayout);
    fillLayout->setAlignment(fillColorLayout, Qt::AlignRight);
    HPushButton *parentButtonFill = new HPushButton();
    parentButtonFill->setIcon(QIcon(":/images/icon_fill_transparent.png"));
    parentButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(parentButtonFill);
    m_mapBrushColorButton[0] = parentButtonFill;
    parentButtonFill->setChecked(true);
    QObject::connect(parentButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 0);
    });

    HPushButton *redButtonFill = new HPushButton();
    redButtonFill->setIcon(QIcon(":/images/icon_fill_red.png"));
    redButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(redButtonFill);
    m_mapBrushColorButton[1] = redButtonFill;
    QObject::connect(redButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 1);
    });
    HPushButton *yellowButtonFill = new HPushButton();
    yellowButtonFill->setIcon(QIcon(":/images/icon_fill_yellow.png"));
    yellowButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(yellowButtonFill);
    m_mapBrushColorButton[2] = yellowButtonFill;
    QObject::connect(yellowButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 2);
    });
    HPushButton *blueButtonFill = new HPushButton();
    blueButtonFill->setIcon(QIcon(":/images/icon_fill_blue.png"));
    blueButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(blueButtonFill);
    m_mapBrushColorButton[3] = blueButtonFill;
    QObject::connect(blueButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 3);
    });
    HPushButton *greenButtonFill = new HPushButton();
    greenButtonFill->setIcon(QIcon(":/images/icon_fill_green.png"));
    greenButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(greenButtonFill);
    m_mapBrushColorButton[4] = greenButtonFill;
    QObject::connect(greenButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 4);
    });
    HPushButton *whiteButtonFill = new HPushButton();
    whiteButtonFill->setIcon(QIcon(":/images/icon_fill_white.png"));
    whiteButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(whiteButtonFill);
    m_mapBrushColorButton[5] = whiteButtonFill;
    QObject::connect(whiteButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 5);
    });
    HPushButton *blackButtonFill = new HPushButton();
    blackButtonFill->setIcon(QIcon(":/images/icon_fill_black.png"));
    blackButtonFill->setIconSize(QSize(32, 32));
    fillColorLayout->addWidget(blackButtonFill);
    m_mapBrushColorButton[6] = blackButtonFill;
    QObject::connect(blackButtonFill, &QPushButton::clicked, [=]{
        OnPropertyChanged(3, 6);
    });

    //橡皮檫粗细
    QHBoxLayout *eraserLayout = new QHBoxLayout();
    mainLayout->addLayout(eraserLayout);
    mainLayout->setStretchFactor(eraserLayout, 2);
    mainLayout->setAlignment(eraserLayout, Qt::AlignTop);
    //-------------------------
    HLabel *eraserTitle = new HLabel();
    eraserTitle->setText(tr("eraserwidth"));
    eraserLayout->addWidget(eraserTitle);
    eraserLayout->setAlignment(eraserTitle, Qt::AlignLeft);
    //--------------------------
    QHBoxLayout *eraserWidthLayout =  new QHBoxLayout();
    eraserLayout->addLayout(eraserWidthLayout);
    eraserLayout->setAlignment(eraserWidthLayout, Qt::AlignRight);
    HPushButton *eraser2Button = new HPushButton();
    eraser2Button->setIcon(QIcon(":/images/icon_eraser_2.png"));
    eraser2Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser2Button);
    m_mapEraserWidthButton[1] = eraser2Button;
    QObject::connect(eraser2Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 1);
    });
    HPushButton *eraser4Button = new HPushButton();
    eraser4Button->setIcon(QIcon(":/images/icon_eraser_4.png"));
    eraser4Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser4Button);
    m_mapEraserWidthButton[2] = eraser4Button;
    QObject::connect(eraser4Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 2);
    });
    HPushButton *eraser8Button = new HPushButton();
    eraser8Button->setIcon(QIcon(":/images/icon_eraser_8.png"));
    eraser8Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser8Button);
    m_mapEraserWidthButton[3] = eraser8Button;
    QObject::connect(eraser8Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 3);
    });
    HPushButton *eraser12Button = new HPushButton();
    eraser12Button->setIcon(QIcon(":/images/icon_eraser_12.png"));
    eraser12Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser12Button);
    m_mapEraserWidthButton[4] = eraser12Button;
    QObject::connect(eraser12Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 4);
    });
    HPushButton *eraser16Button = new HPushButton();
    eraser16Button->setIcon(QIcon(":/images/icon_eraser_16.png"));
    eraser16Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser16Button);
    eraser16Button->setChecked(true);
    m_mapEraserWidthButton[5] = eraser16Button;
    QObject::connect(eraser16Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 5);
    });
    HPushButton *eraser20Button = new HPushButton();
    eraser20Button->setIcon(QIcon(":/images/icon_eraser_20.png"));
    eraser20Button->setIconSize(QSize(32, 32));
    eraserWidthLayout->addWidget(eraser20Button);
    m_mapEraserWidthButton[6] = eraser20Button;
    QObject::connect(eraser20Button, &QPushButton::clicked, [=]{
        OnPropertyChanged(4, 6);
    });
}

void HPropertyWidget::onCloseButtonClicked()
{

    hide();
}

void HPropertyWidget::OnPropertyChanged(int type, int value)
{
    switch(type){
    case 1:{
        QColor color = PenColorFromIndex(value);
        for(QMap<int, HPushButton *>::iterator it = m_mapPenColorButton.begin();
            it != m_mapPenColorButton.end(); it ++){
            if(it.key() == value){
                it.value()->setChecked(true);
            }else{
                it.value()->setChecked(false);
            }
        }
        emit sigPenColorChanged(value, color);
        break;
    }
    case 2:{
        int width = PenWidthFromIndex(value);
        for(QMap<int, HPushButton *>::iterator it = m_mapPenWidthButton.begin();
            it != m_mapPenWidthButton.end(); it ++){
            if(it.key() == value){
                it.value()->setChecked(true);
            }else{
                it.value()->setChecked(false);
            }
        }
        emit sigPenWidthChanged(value, width);
        break;
    }
    case 3:{
        QColor color = BrushColorFromIndex(value);
        for(QMap<int, HPushButton *>::iterator it = m_mapBrushColorButton.begin();
            it != m_mapBrushColorButton.end(); it ++){
            if(it.key() == value){
                it.value()->setChecked(true);
            }else{
                it.value()->setChecked(false);
            }
        }
        emit sigBrushColorChanged(value, color);
        break;
    }
    case 4:{
        int width = EraserWidthFromIndex(value);
        for(QMap<int, HPushButton *>::iterator it = m_mapEraserWidthButton.begin();
            it != m_mapEraserWidthButton.end(); it ++){
            if(it.key() == value){
                it.value()->setChecked(true);
            }else{
                it.value()->setChecked(false);
            }
        }
        emit sigEraserWidthChanged(value, width);
        break;
    }
    }
}

void HPropertyWidget::SetDefaultValue(){
    int penColorIndex = HPropertyObject::getInstance()->PenColorIndex();
    for(QMap<int, HPushButton *>::iterator it = m_mapPenColorButton.find(penColorIndex);
        it != m_mapPenColorButton.end(); it ++){
        if(it.key() == penColorIndex){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
    int penWidthIndex = HPropertyObject::getInstance()->PenWidthIndex();
    for(QMap<int, HPushButton *>::iterator it = m_mapPenWidthButton.find(penColorIndex);
        it != m_mapPenWidthButton.end(); it ++){
        if(it.key() == penWidthIndex){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
    int brushColorIndex = HPropertyObject::getInstance()->BrushColorIndex();
    for(QMap<int, HPushButton *>::iterator it = m_mapBrushColorButton.find(penColorIndex);
        it != m_mapBrushColorButton.end(); it ++){
        if(it.key() == brushColorIndex){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
    int eraserWidthIndex = HPropertyObject::getInstance()->EraserWidthIndex();
    for(QMap<int, HPushButton *>::iterator it = m_mapEraserWidthButton.find(penColorIndex);
        it != m_mapEraserWidthButton.end(); it ++){
        if(it.key() == eraserWidthIndex){
            it.value()->setChecked(true);
        }else{
            it.value()->setChecked(false);
        }
    }
}

void HPropertyWidget::showEvent(QShowEvent *event)
{
    this->setStyleSheet("HPropertyWidget{background-color: rgba(0, 0, 0, 75%);}");
}

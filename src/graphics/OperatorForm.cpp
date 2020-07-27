#include "OperatorForm.h"
#include "ui_OperatorForm.h"

OperatorForm::OperatorForm(int key, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OperatorForm)
    , m_nKey(key)
{
    ui->setupUi(this);
}

OperatorForm::~OperatorForm()
{
    delete ui;
}

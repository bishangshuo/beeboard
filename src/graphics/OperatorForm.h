#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>

namespace Ui {
class OperatorForm;
}

class OperatorForm : public QWidget
{
    Q_OBJECT

public:
    explicit OperatorForm(int key, QWidget *parent = nullptr);
    ~OperatorForm();

private:
    Ui::OperatorForm *ui;
    int m_nKey;
};

#endif // OPERATORFORM_H

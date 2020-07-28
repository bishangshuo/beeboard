/********************************************************************************
** Form generated from reading UI file 'OperatorForm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPERATORFORM_H
#define UI_OPERATORFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OperatorForm
{
public:

    void setupUi(QWidget *OperatorForm)
    {
        if (OperatorForm->objectName().isEmpty())
            OperatorForm->setObjectName(QString::fromUtf8("OperatorForm"));
        OperatorForm->resize(148, 146);
        OperatorForm->setMinimumSize(QSize(80, 80));

        retranslateUi(OperatorForm);

        QMetaObject::connectSlotsByName(OperatorForm);
    } // setupUi

    void retranslateUi(QWidget *OperatorForm)
    {
        OperatorForm->setWindowTitle(QCoreApplication::translate("OperatorForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OperatorForm: public Ui_OperatorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPERATORFORM_H

/********************************************************************************
** Form generated from reading UI file 'LoginWin.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIN_H
#define UI_LOGINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWin
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *ID_Line;
    QLabel *label_2;
    QLineEdit *PW_Line;
    QPushButton *Login_Button;
    QCheckBox *anonymous_check;

    void setupUi(QWidget *LoginWin)
    {
        if (LoginWin->objectName().isEmpty())
            LoginWin->setObjectName(QStringLiteral("LoginWin"));
        LoginWin->resize(294, 173);
        gridLayout = new QGridLayout(LoginWin);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        formLayout->setContentsMargins(40, 40, 40, 40);
        label = new QLabel(LoginWin);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        ID_Line = new QLineEdit(LoginWin);
        ID_Line->setObjectName(QStringLiteral("ID_Line"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ID_Line);

        label_2 = new QLabel(LoginWin);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        PW_Line = new QLineEdit(LoginWin);
        PW_Line->setObjectName(QStringLiteral("PW_Line"));
        PW_Line->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, PW_Line);

        Login_Button = new QPushButton(LoginWin);
        Login_Button->setObjectName(QStringLiteral("Login_Button"));

        formLayout->setWidget(2, QFormLayout::FieldRole, Login_Button);

        anonymous_check = new QCheckBox(LoginWin);
        anonymous_check->setObjectName(QStringLiteral("anonymous_check"));
        anonymous_check->setLayoutDirection(Qt::LeftToRight);
        anonymous_check->setAutoRepeat(false);
        anonymous_check->setTristate(false);

        formLayout->setWidget(2, QFormLayout::LabelRole, anonymous_check);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(LoginWin);
        QObject::connect(PW_Line, SIGNAL(returnPressed()), Login_Button, SLOT(click()));

        QMetaObject::connectSlotsByName(LoginWin);
    } // setupUi

    void retranslateUi(QWidget *LoginWin)
    {
        LoginWin->setWindowTitle(QApplication::translate("LoginWin", "LoginWin", Q_NULLPTR));
        label->setText(QApplication::translate("LoginWin", "\347\224\250\346\210\267\345\220\215:", Q_NULLPTR));
        ID_Line->setPlaceholderText(QApplication::translate("LoginWin", "\345\234\250\346\255\244\350\274\270\345\205\245\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("LoginWin", "\345\257\206\347\240\201:", Q_NULLPTR));
        PW_Line->setInputMask(QString());
        PW_Line->setText(QString());
        PW_Line->setPlaceholderText(QApplication::translate("LoginWin", "\345\234\250\346\255\244\350\274\270\345\205\245\345\257\206\347\240\201", Q_NULLPTR));
        Login_Button->setText(QApplication::translate("LoginWin", "\347\231\273\345\275\225(&Eeter)", Q_NULLPTR));
        anonymous_check->setText(QApplication::translate("LoginWin", "\345\214\277\345\220\215\347\231\273\345\275\225", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LoginWin: public Ui_LoginWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIN_H

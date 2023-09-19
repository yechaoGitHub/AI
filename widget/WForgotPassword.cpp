#include "WForgotPassword.h"

WForgotPassword::WForgotPassword(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/user_active.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code in picture");

    ui.edPhone->SetImage(":/QtTest/icon/lock.png");
    ui.edPhone->textEdit->setPlaceholderText("Enter phone number");

    ui.vCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.vCodeEdit->textEdit->setPlaceholderText("Enter the code you received");

    verifyBtn = ui.pbVerify;
}

WForgotPassword::~WForgotPassword()
{
}

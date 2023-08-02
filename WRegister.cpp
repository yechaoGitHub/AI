#include "WRegister.h"

WRegister::WRegister(QWidget* parent) :
    QWidget{ parent },
    signBtn{ nullptr }
{
    ui.setupUi(this);
    ui.verfyCodeWidget->loggedCheckBox->hide();

    ui.userNameEdit->SetImage(":/QtTest/icon/user_active.png");
    ui.userNameEdit->textEdit->setPlaceholderText("Enter user name");

    ui.passwordEdit->SetImage(":/QtTest/icon/lock.png");
    ui.passwordEdit->textEdit->setPlaceholderText("Enter password");

    ui.passwordEdit2->SetImage(":/QtTest/icon/lock.png");
    ui.passwordEdit2->textEdit->setPlaceholderText("Enter password again");

    ui.signBtn->SetText("Sign Up");
    ui.signBtn->setCursor(Qt::PointingHandCursor);

    signBtn = ui.signBtn;
}

WRegister::~WRegister()
{
}

QString WRegister::UserName()
{
    return ui.userNameEdit->textEdit->text();
}

QString WRegister::Password()
{
    return ui.passwordEdit->textEdit->text();
}

QString WRegister::PhoneNumber()
{
    return ui.verfyCodeWidget->PhoneNumber();
}

QString WRegister::VerifyCode()
{
    return ui.verfyCodeWidget->VerifyCode();
}

bool WRegister::Check()
{
    return false;
}

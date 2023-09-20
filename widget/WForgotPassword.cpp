#include "WForgotPassword.h"

#include "AiSound.h"

#include <QListView>

WForgotPassword::WForgotPassword(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.cbPhone->setView(new QListView{});

    ui.edUser->SetImage(":/QtTest/icon/user_active.png");
    ui.edUser->textEdit->setPlaceholderText("Enter user name");

    ui.edPassword->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword->textEdit->setPlaceholderText("Enter password");

    ui.edPassword2->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword2->textEdit->setPlaceholderText("Enter password again");

    verifyBtn = ui.pbVerfy;
}

WForgotPassword::~WForgotPassword()
{
}

QString WForgotPassword::DialingCode()
{
    auto index = ui.cbPhone->currentIndex();
    QString dialingCode;
    if (index != -1)
    {
        dialingCode = ui.cbPhone->itemData(index).toString();
        return dialingCode;
    }
    else
    {
        return "";
    }

    return "";
}

QString WForgotPassword::UserName()
{
    return ui.edUser->textEdit->text();
}

QString WForgotPassword::Password()
{
    return ui.edPassword->textEdit->text();
}

QString WForgotPassword::Repassword()
{
    return ui.edPassword2->textEdit->text();
}

QString WForgotPassword::VerifyCode()
{
    return ui.edVCode->text();
}

void WForgotPassword::showEvent(QShowEvent* event)
{
    if (ui.cbPhone->count() == 0)
    {
        auto& ai = AiSound::GetInstance();
        const auto& phoneData = ai.GetPhoneRegionInfo();
        for (auto& data : phoneData)
        {
            QString iconPath = ":/QtTest/icon/country/" + data.abb + ".png";
            ui.cbPhone->setIconSize(QSize{ 32, 16 });
            ui.cbPhone->addItem(QIcon{ iconPath }, data.name, data.dialingCode);
        }
    }
}

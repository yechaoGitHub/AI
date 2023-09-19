#include "WForgotPassword.h"

#include "AiSound.h"

#include <QListView>

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

    ui.cbPhone->setView(new QListView{});

    verifyBtn = ui.pbVerify;
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
    return ui.edPhone->textEdit->text();
}

QString WForgotPassword::Password()
{
    return QString();
}

QString WForgotPassword::Repassword()
{
    return QString();
}

QString WForgotPassword::VerifyCode()
{
    return QString();
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

#include "WRegister.h"
#include "AiSound.h"

WRegister::WRegister(QWidget* parent) :
    QWidget{ parent },
    signBtn{ nullptr }
{
    ui.setupUi(this);

    ui.userNameEdit->SetImage(":/QtTest/icon/user_active.png");
    ui.userNameEdit->textEdit->setPlaceholderText("Enter user name");

    ui.passwordEdit->SetImage(":/QtTest/icon/lock.png");
    ui.passwordEdit->textEdit->setPlaceholderText("Enter password");

    ui.passwordEdit2->SetImage(":/QtTest/icon/lock.png");
    ui.passwordEdit2->textEdit->setPlaceholderText("Enter password again");

    signBtn = ui.pbSignUp;
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
    return ui.userNameEdit->textEdit->text();
}

QString WRegister::VerifyCode()
{
    return ui.verificationCodeEdit->textEdit->text();
}

bool WRegister::Check()
{
    return false;
}

void WRegister::showEvent(QShowEvent* event)
{
    static bool firstShow = true;
    if (firstShow)
    {
        ui.verificationCodePic->FlushVCode();
        firstShow = false;
    }

    if (ui.comboBox->count() == 0)
    {
        auto& ai = AiSound::GetInstance();
        const auto& phoneData = ai.GetPhoneRegionInfo();
        for (auto& data : phoneData)
        {
            QString iconPath = ":/QtTest/icon/country/" + data.abb + ".png";
            ui.comboBox->setIconSize(QSize{ 32, 16 });
            ui.comboBox->addItem(QIcon{ iconPath }, data.name, data.dialingCode);
        }
    }
}

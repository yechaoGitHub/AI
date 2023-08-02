#include "WMobileLogin.h"
#include "AiSound.h"

WMobileLogin::WMobileLogin(QWidget* parent) :
    QWidget{ parent },
    loggedCheckBox{ nullptr }
{
    ui.setupUi(this);

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code in picture");

    ui.getCodeBtn->SetText("Get Code");

    ui.estimatedLable->setStyleSheet("color:rgba(155, 155, 155, 1)");
    ui.resendLabel->setStyleSheet("color:rgba(155, 155, 155, 1)");

    ui.codeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code send on your mobile");

    ui.loggedCheckBox->SetText("Keep me logged in");
    connect(ui.getCodeBtn, &WButton::clicked, this, &WMobileLogin::GetCodeCallback);

    loggedCheckBox = ui.loggedCheckBox;
}

WMobileLogin::~WMobileLogin()
{
}

QString WMobileLogin::PhoneNumber()
{
    return ui.mobileNumEdit->textEdit->text();
}

QString WMobileLogin::VerifyCode()
{
    return ui.verificationCodeEdit->textEdit->text();
}

void WMobileLogin::GetCodeCallback()
{
    auto& ins = AiSound::GetInstance();

    auto phoneNumber = ui.mobileNumEdit->textEdit->text();
    auto phpneCode = ui.codeEdit->textEdit->text();
    auto verifyCode = ui.verificationCodeEdit->textEdit->text();
    auto uuid = ui.verificationCodePic->Uuid();

    auto f = [](int code, const QString& msg)->void
    {
        qDebug() << msg;
    };

    ins.SendVerifyCode("+86", phoneNumber, verifyCode, uuid, std::bind(f, std::placeholders::_1, std::placeholders::_2));
}

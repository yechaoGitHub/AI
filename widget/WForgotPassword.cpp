#include "WForgotPassword.h"

#include "AiSound.h"

#include <QListView>

WForgotPassword::WForgotPassword(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.cbPhone->setView(new QListView{});

    ui.edUser->SetImage(":/QtTest/icon/user_active.png");
    ui.edUser->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter email address", nullptr));

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter the code in picture", nullptr));
    ui.edVCode->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter the verification code", nullptr));

    ui.edPassword->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter password", nullptr));

    ui.edPassword2->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword2->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter password again", nullptr));

    ui.verificationCodePic->SetModuleType("modifyPwd");

    connect(ui.getCodeBtn, &QPushButton::clicked, this, &WForgotPassword::GetVCodeClicked);
    connect(ui.cbPhone, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WForgotPassword::CountryChanged);

    verifyBtn = ui.pbVerfy;

    CountryChanged(0);

    ui.getCodeBtn->setProperty("enable", true);
    ui.getCodeBtn->style()->unpolish(ui.getCodeBtn);

    ui.lbEstimated->setVisible(false);
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

void WForgotPassword::Clear()
{
    ui.cbPhone->setCurrentIndex(0);
    ui.edUser->textEdit->clear();
    ui.verificationCodeEdit->textEdit->clear();
    ui.edVCode->clear();
    ui.edPassword->textEdit->clear();
    ui.edPassword2->textEdit->clear();
}

void WForgotPassword::showEvent(QShowEvent* event)
{
    static bool firstShow = true;
    if (firstShow)
    {
        ui.verificationCodePic->FlushVCode();
        firstShow = false;
    }

    if (ui.cbPhone->count() == 0)
    {
        auto& ai = AiSound::GetInstance();
        const auto& phoneData = ai.GetPhoneRegionInfo();
        for (auto& data : phoneData)
        {
            QString iconPath = ":/QtTest/icon/country/" + data.abb + ".png";
            ui.cbPhone->setIconSize(QSize{ 32, 16 });
            QString itemName = data.name + " " + data.dialingCode;
            ui.cbPhone->addItem(QIcon{ iconPath }, itemName, data.dialingCode);
        }
    }
}

void WForgotPassword::timerEvent(QTimerEvent* event)
{
    ui.lbEstimated->setVisible(true);
    QString text{ tr("Estimated time of code in %1 seconds") };
    text = text.arg(_downCount);
    ui.lbEstimated->setText(text);

    _downCount--;
    if (_downCount == 0)
    {
        killTimer(_timer);
        ui.lbEstimated->setVisible(false);
        ui.getCodeBtn->setEnabled(true);
        ui.getCodeBtn->setProperty("enable", true);
        ui.getCodeBtn->style()->unpolish(ui.getCodeBtn);
    }
}

void WForgotPassword::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
        ui.edPassword->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter password", nullptr));
        ui.edPassword2->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter password again", nullptr));
        ui.verificationCodeEdit->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter the code in picture", nullptr));
        ui.edVCode->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter the verification code", nullptr));
        ui.edPassword->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword","Enter password", nullptr));
        ui.edPassword2->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword", "Enter password again", nullptr));
        update();
    }

    QWidget::changeEvent(event);
}

void WForgotPassword::GetVCodeClicked()
{
    auto& ins = AiSound::GetInstance();

    QString phoneNumber = ui.edUser->textEdit->text();
    auto verifyCode = ui.verificationCodeEdit->textEdit->text();
    auto uuid = ui.verificationCodePic->Uuid();
    auto dialingCode = DialingCode();

    auto callback = [this](int code, const QString& msg)
    {
        if (code != 200)
        {
            auto& ins = AiSound::GetInstance();
            ins.ShowTip(this, msg);
        }
        else
        {
            _downCount = 60;
            _timer = startTimer(1000);
            ui.getCodeBtn->setEnabled(false);
            ui.getCodeBtn->setProperty("enable", false);
            ui.getCodeBtn->style()->unpolish(ui.getCodeBtn);
        }
    };

    if (dialingCode == "+86")
    {
        ins.SendVerifyCode(dialingCode, phoneNumber, verifyCode, uuid, "modifyPwd", callback);
    }
    else
    {
        ins.SendMailVerfyCode(phoneNumber, verifyCode, uuid, "modifyPwd", callback);
    }

}

void WForgotPassword::CountryChanged(int index)
{
    if (DialingCode() == "+86")
    {
        ui.edUser->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword","Enter the mobile phone",nullptr));
    }
    else
    {
        ui.edUser->textEdit->setPlaceholderText(QCoreApplication::translate("WForgotPassword","Enter the email address",nullptr));
    }
}

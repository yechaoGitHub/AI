#include "WMobileLogin.h"
#include "AiSound.h"
#include <QlistView>

WMobileLogin::WMobileLogin(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.comboBox->setView(new QListView{});

    ui.codeEdit->textEdit->setPlaceholderText("Enter the email address");

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code in picture");

    ui.lineEdit->setPlaceholderText(tr("Enter the code send on your email"));

    connect(ui.getCodeBtn, &QPushButton::clicked, this, &WMobileLogin::GetCodeCallback);

    ui.lbEstimated->setVisible(false);
}

WMobileLogin::~WMobileLogin()
{
}

QString WMobileLogin::PhoneNumber()
{
    return ui.codeEdit->textEdit->text();
}

QString WMobileLogin::VerifyCode()
{
    return ui.verificationCodeEdit->textEdit->text();
}

QString WMobileLogin::DialingCode()
{
    auto index = ui.comboBox->currentIndex();
    QString dialingCode;
    if (index != -1)
    {
        dialingCode = ui.comboBox->itemData(index).toString();
        return dialingCode;
    }
    else
    {
        return "";
    }

    return "";
}

void WMobileLogin::timerEvent(QTimerEvent* event)
{
    ui.lbEstimated->setVisible(true);
    QString text = QString::fromLocal8Bit("Estimated time of code in %1 seconds").arg(_downCount);
    ui.lbEstimated->setText(text);

    _downCount--;
    if (_downCount == 0)
    {
        killTimer(_timer);
        ui.lbEstimated->setVisible(false);
        ui.getCodeBtn->setEnabled(true);
    }
}

void WMobileLogin::showEvent(QShowEvent* event)
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
            ui.comboBox->setIconSize(QSize{32, 16});
            ui.comboBox->addItem(QIcon{ iconPath }, data.name, data.dialingCode);
        }
    }
}

void WMobileLogin::GetCodeCallback()
{
    auto& ins = AiSound::GetInstance();

    QString phoneNumber = ui.codeEdit->textEdit->text();
    auto phpneCode = ui.codeEdit->textEdit->text();
    auto verifyCode = ui.verificationCodeEdit->textEdit->text();
    auto uuid = ui.verificationCodePic->Uuid();
    auto index = ui.comboBox->currentIndex();
    QString dialingCode;
    if (index != -1)
    {
        dialingCode = ui.comboBox->itemData(index).toString();
    }

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
        }
    };

    ins.SendVerifyCode(dialingCode, phoneNumber, verifyCode, uuid, callback);
}

#include "WMobileLogin.h"
#include "AiSound.h"
#include <QlistView>

WMobileLogin::WMobileLogin(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.comboBox->setView(new QListView{});

    ui.codeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.codeEdit->textEdit->setPlaceholderText(tr("Enter the email address"));

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText(tr("Enter the code in picture"));

    ui.lineEdit->setPlaceholderText(tr("Enter the code send on your email"));

    auto& ai = AiSound::GetInstance();

    connect(ui.getCodeBtn, &QPushButton::clicked, this, &WMobileLogin::GetCodeCallback);
    connect(ui.comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WMobileLogin::CountryChanged);
    connect(&ai, &AiSound::languageDataChanged, this, &WMobileLogin::ReloadPhoneNumer);


    ui.lbEstimated->setVisible(false);

    ui.getCodeBtn->setProperty("enable", true);
    ui.getCodeBtn->style()->unpolish(ui.getCodeBtn);

    ui.verificationCodePic->SetModuleType("login");

    CountryChanged(0);
}

WMobileLogin::~WMobileLogin()
{
}

QString WMobileLogin::PhoneNumber()
{
    return ui.codeEdit->textEdit->text();
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

QString WMobileLogin::PhoneCode()
{
    return ui.lineEdit->text();
}

void WMobileLogin::timerEvent(QTimerEvent* event)
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
            QString itemName = data.name + " " + data.dialingCode;
            ui.comboBox->addItem(QIcon{ iconPath }, itemName, data.dialingCode);
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
            ui.getCodeBtn->setProperty("enable", false);
            ui.getCodeBtn->style()->unpolish(ui.getCodeBtn);
        }
    };

    if (DialingCode() == "+86")
    {
        ins.SendVerifyCode(dialingCode, phoneNumber, verifyCode, uuid, "login", callback);
    }
    else
    {
        ins.SendMailVerfyCode(phoneNumber, verifyCode, uuid, "login", callback);
    }
}

void WMobileLogin::CountryChanged(int index)
{
    _selectCode = DialingCode();

    if (_selectCode == "+86")
    {
        ui.codeEdit->textEdit->setPlaceholderText(tr("Enter the mobile phone"));
    }
    else
    {
        ui.codeEdit->textEdit->setPlaceholderText(tr("Enter the email address"));
    }
}

void WMobileLogin::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);

        ui.codeEdit->textEdit->setPlaceholderText(tr("Enter the email address"));
        ui.verificationCodeEdit->textEdit->setPlaceholderText(tr("Enter the code in picture"));
        ui.lineEdit->setPlaceholderText(tr("Enter the code send on your email"));
        //ReloadPhoneNumer();
    }

    QWidget::changeEvent(event);
}

void WMobileLogin::ReloadPhoneNumer()
{
    ui.comboBox->clear();

    const auto& phoneData = AiSound::GetInstance().GetPhoneRegionInfo();
    auto index = 0;
    for (auto& data : phoneData)
    {
        QString iconPath = ":/QtTest/icon/country/" + data.abb + ".png";
        ui.comboBox->setIconSize(QSize{ 32, 16 });
        QString itemName = data.name + " " + data.dialingCode;
        ui.comboBox->addItem(QIcon{ iconPath }, itemName, data.dialingCode);

        if (data.dialingCode == _selectCode)
        {
            ui.comboBox->setCurrentIndex(index);
        }
        index++;
    }
}

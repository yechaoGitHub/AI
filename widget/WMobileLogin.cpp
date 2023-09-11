#include "WMobileLogin.h"
#include "AiSound.h"
#include <qstyle.h>


WMobileLogin::WMobileLogin(QWidget* parent) :
    QWidget{ parent }//,
    //loggedCheckBox{ nullptr }
{
    ui.setupUi(this);

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code in picture");

    ui.getCodeBtn->SetText("Get Code");

    ui.estimatedLable->setStyleSheet("color:rgba(155, 155, 155, 1)");
    ui.resendLabel->setStyleSheet("color:rgba(155, 155, 155, 1)");

    ui.codeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText("Enter the code send on your mobile");

    //ui.loggedCheckBox->SetText("Keep me logged in");
    //ui.loggedCheckBox->hide();
    connect(ui.getCodeBtn, &WButton::clicked, this, &WMobileLogin::GetCodeCallback);

    //loggedCheckBox = ui.loggedCheckBox;

    ui.comboBox->setStyleSheet(
        "QComboBox{border:1px red solid;}QComboBox::drop-down "
        "{width:0px;border:1px red solid;}"
        "QComboBox::down-arrow{image:none;}");
    ui.comboBox->setIconSize(QSize(16, 16));
    ui.comboBox->addItem(QIcon(":/QtTest/icon/China.png"),"+86");
    ui.lineEdit->setPlaceholderText(tr("Enter your phone"));

    ui.frame->setStyleSheet("QFrame#frame{border:1px solid #e5e5e5;border-radius:8px;}; ");
    connect(ui.lineEdit, &QLineEdit::textChanged, this, [=](const QString& phone) {
        if (phone.isEmpty()) {
            ui.frame->setStyleSheet("QFrame#frame{border:1px solid #e5e5e5;border-radius:8px;}; ");
        }
        else {
            ui.frame->setStyleSheet("QFrame#frame{border:1px solid qlineargradient(spread : pad, x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #0066FF, stop:1 #BD00FF);border-radius:8px;}; ");
        }
        });
    connect(ui.pushButton, &QPushButton::clicked, this, [=] {
        ui.comboBox->showPopup();
        });
}

WMobileLogin::~WMobileLogin()
{
}

QString WMobileLogin::PhoneNumber()
{
    //return ui.mobileNumEdit->textEdit->text();
    return "";
}

QString WMobileLogin::VerifyCode()
{
    return ui.verificationCodeEdit->textEdit->text();
}

void WMobileLogin::GetCodeCallback()
{
    auto& ins = AiSound::GetInstance();

    QString phoneNumber;// = ui.mobileNumEdit->textEdit->text();
    auto phpneCode = ui.codeEdit->textEdit->text();
    auto verifyCode = ui.verificationCodeEdit->textEdit->text();
    auto uuid = ui.verificationCodePic->Uuid();

    auto f = [](int code, const QString& msg)->void
    {
        qDebug() << msg;
    };

    ins.SendVerifyCode("+86", phoneNumber, verifyCode, uuid, std::bind(f, std::placeholders::_1, std::placeholders::_2));
}

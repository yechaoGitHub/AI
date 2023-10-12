#include "WRegister.h"
#include "AiSound.h"

#include <QListView>

WRegister::WRegister(QWidget* parent) :
    QWidget{ parent },
    signBtn{ nullptr }
{
    ui.setupUi(this);

    ui.cbPhone->setView(new QListView{});

    ui.edUser->SetImage(":/QtTest/icon/user_active.png");
    ui.edUser->textEdit->setPlaceholderText(tr("Enter email address"));

    ui.verificationCodeEdit->SetImage(":/QtTest/icon/lock.png");
    ui.verificationCodeEdit->textEdit->setPlaceholderText(tr("Enter the code in picture"));
    ui.edVCode->setPlaceholderText(tr("Enter the verification code"));

    ui.edPassword->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword->textEdit->setPlaceholderText("Enter password");

    ui.edPassword2->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword2->textEdit->setPlaceholderText("Enter password again");

    ui.edRecommend->SetImage(":/QtTest/icon/lock.png");
    ui.edRecommend->textEdit->setPlaceholderText("(Optional) Enter the invitation code");

    ui.verificationCodePic->SetModuleType("register");

    connect(ui.getCodeBtn, &QPushButton::clicked, this, &WRegister::GetCodeCallback);
    connect(ui.cbPhone, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WRegister::CountryChanged);

    signBtn = ui.pbSignUp;

    CountryChanged(0);
}

WRegister::~WRegister()
{
}

QString WRegister::UserName()
{
    return ui.edUser->textEdit->text();
}

QString WRegister::Password()
{
    return ui.edPassword->textEdit->text();
}

QString WRegister::Repassword()
{
    return ui.edPassword2->textEdit->text();
}

QString WRegister::VerifyCode()
{
    return ui.edVCode->text();
}

QString WRegister::DialingCode()
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


QString WRegister::RecommondCode()
{
    return ui.edRecommend->textEdit->text();
}

void WRegister::showEvent(QShowEvent* event)
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

void WRegister::GetCodeCallback()
{
    auto& ins = AiSound::GetInstance();

    auto&& phoneNumber = UserName();
    auto verifyCode = ui.verificationCodeEdit->textEdit->text();
    auto uuid = ui.verificationCodePic->Uuid();
    auto index = ui.cbPhone->currentIndex();
    QString dialingCode;
    if (index != -1)
    {
        dialingCode = ui.cbPhone->itemData(index).toString();
    }

    auto callback = [this](int code, const QString& msg)
    {
        if (code != 200)
        {
            auto& ins = AiSound::GetInstance();
            ins.ShowTip(this, msg);
        }
    };

    if (DialingCode() == "+86")
    {
        ins.SendVerifyCode(dialingCode, phoneNumber, verifyCode, uuid, "register", callback);
    }
    else
    {
        ins.SendMailVerfyCode(phoneNumber, verifyCode, uuid, "register", callback);
    }
}

void WRegister::CountryChanged(int index)
{
    if (DialingCode() == "+86")
    {
        ui.edUser->textEdit->setPlaceholderText(tr("Enter the mobile phone"));
    }
    else
    {
        ui.edUser->textEdit->setPlaceholderText(tr("Enter the email address"));
    }
}

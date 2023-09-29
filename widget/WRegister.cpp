#include "WRegister.h"
#include "AiSound.h"

#include <QListView>

WRegister::WRegister(QWidget* parent) :
    QWidget{ parent },
    signBtn{ nullptr }
{
    ui.setupUi(this);

    ui.cbPhone->setView(new QListView{});

    ui.edRecommend->textEdit->setPlaceholderText("(Optional) Enter the invitation code. If not please leave blank");

    ui.edUser->SetImage(":/QtTest/icon/user_active.png");
    ui.edUser->textEdit->setPlaceholderText("Enter user name");

    ui.edPassword->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword->textEdit->setPlaceholderText("Enter password");

    ui.edPassword2->SetImage(":/QtTest/icon/lock.png");
    ui.edPassword2->textEdit->setPlaceholderText("Enter password again");

    signBtn = ui.pbSignUp;

    ui.verificationCodePic->SetModuleType("register");

    connect(ui.getCodeBtn, &QPushButton::clicked, this, &WRegister::GetCodeCallback);
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

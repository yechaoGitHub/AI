#include "WLogin.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"

WLogin::WLogin(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    TitleChanged(WLoginSwitch::ETitle::userName);

    ui.userLogin->userNameEdit->setPlaceholderText(tr("Enter the username"));
    ui.userLogin->passwordEdit->setPlaceholderText(tr("Enter the password"));

    registerLabel = ui.registerBtn;
    loginBtn = ui.loginBtn;
    cbRemeber = ui.cbKeepLogin;
    forgotPasswordBtn = ui.pbForgotPassword;

    auto userName = UserName();

    if (SETTING.getRememberPWD())
    {
        auto password = SETTING.getPWD();
        auto userName = SETTING.getUserName();

        ui.userLogin->userNameEdit->setText(userName);
        ui.userLogin->passwordEdit->setText(password);
        ui.cbKeepLogin->setChecked(true);
    }

    connect(ui.btLanguageText, &QPushButton::clicked, this, &WLogin::LanguageClicked);
    connect(ui.loginSwitch, &WLoginSwitch::TitleChanged, this, &WLogin::TitleChanged);
    connect(ui.loginBtn, &QPushButton::clicked, this, &WLogin::LoginClicked);
}

WLogin::~WLogin()
{

}

bool  WLogin::remberPwd()
{
    return ui.cbKeepLogin->isChecked();
}

QString WLogin::UserName()
{
    return ui.userLogin->userNameEdit->text();
}

QString WLogin::Password()
{
    return ui.userLogin->passwordEdit->text();
}

QString WLogin::DialingCode()
{
    return ui.mobileLogin->DialingCode();
}

QString WLogin::VerfyCode()
{
    return ui.mobileLogin->PhoneCode();
}

void WLogin::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
        ui.userLogin->userNameEdit->setPlaceholderText(tr("Enter the username"));
        ui.userLogin->passwordEdit->setPlaceholderText(tr("Enter the password"));
    }

    QWidget::changeEvent(event);
}

void WLogin::LoginClicked()
{
    switch (ui.loginStackedWidget->currentIndex())
    {
        case 0:
            PasswordLogin();
        break;

        case 1:
            if (ui.mobileLogin->DialingCode() ==
                QString::fromLocal8Bit("+86"))
            {
                PhoneLogin();
            }
            else
            {
                EmailLogin();
            }
        break;

        default:
        break;
    }
}

void WLogin::TitleChanged(WLoginSwitch::ETitle title)
{
    _cur_login_title = title;
    if (title == WLoginSwitch::ETitle::userName)
    {
        ui.loginStackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui.loginStackedWidget->setCurrentIndex(1);
    }
}

void WLogin::LanguageClicked()
{
    auto& ins = AiSound::GetInstance();

    switch (ins.GetSystemLanguage())
    {
        case LanguageType::EN:
            ins.SwitchLanguage(LanguageType::CHS);
        break;

        case LanguageType::CHS:
            ins.SwitchLanguage(LanguageType::EN);
        break;
    }
}

void WLogin::PasswordLogin()
{
    auto& ins = AiSound::GetInstance();
    auto callback = [this](int code, const QString& msg, const QString& token)->void
    {
        auto& ins = AiSound::GetInstance();

        if (code == 200)
        {
            auto userName = UserName();
            auto password = Password();
            if (remberPwd())
            {
                SETTING.setPWD(password);
                SETTING.setUserName(userName);
            }
            else
            {
                SETTING.setPWD("");
                SETTING.setUserName("");
            }
            SETTING.setRememberPWD(remberPwd());

            ins.ShowRobotNavigation();
        }
        else
        {
            ins.ShowTip(this, msg);
        }
    };
    ins.PasswordLogin(UserName(), Password(), std::bind(callback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void WLogin::EmailLogin()
{
    auto& ins = AiSound::GetInstance();

    auto&& phoneNum = ui.mobileLogin->PhoneNumber();
    auto&& vCode = ui.mobileLogin->PhoneCode();

    auto callback = [this](int code, const QString& msg, const QString& token)->void
    {
        auto& ins = AiSound::GetInstance();

        if (code == 200)
        {
            ins.ShowRobotNavigation();
        }
        else
        {
            ins.ShowTip(this, msg);
        }
    };

    ins.EmailLogin(phoneNum, vCode, callback);
}

void WLogin::PhoneLogin()
{
    auto& ins = AiSound::GetInstance();

    auto&& dCode = ui.mobileLogin->DialingCode();
    auto&& phoneNum = ui.mobileLogin->PhoneNumber();
    auto&& vCode = ui.mobileLogin->PhoneCode();

    auto callback = [this](int code, const QString& msg, const QString& token)->void
    {
        auto& ins = AiSound::GetInstance();

        if (code == 200)
        {
            ins.ShowRobotNavigation();
        }
        else
        {
            ins.ShowTip(this, msg);
        }
    };

    ins.PhoneLogin(dCode, phoneNum, vCode, callback);
}

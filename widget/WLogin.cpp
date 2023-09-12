#include "WLogin.h"
#include "AiSound.h"

WLogin::WLogin(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    TitleChanged(WLoginSwitch::ETitle::userName);

    registerLabel = ui.registerBtn;
    loginBtn = ui.loginBtn;
    cbRemeber = ui.cbKeepLogin;

    connect(ui.btLanguageText, &QPushButton::clicked, this, &WLogin::LanguageClicked);
    connect(ui.loginSwitch, &WLoginSwitch::TitleChanged, this, &WLogin::TitleChanged);
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

void WLogin::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {

    }

    QWidget::changeEvent(event);
}

void WLogin::TitleChanged(WLoginSwitch::ETitle title)
{
    _cur_login_title = title;
    if (title == WLoginSwitch::ETitle::userName)
    {
        ui.stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui.stackedWidget->setCurrentIndex(1);
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



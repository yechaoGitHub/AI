#include "WLogin.h"
#include "AiSound.h"

WLogin::WLogin(QWidget* parent) :
    QWidget{ parent }//,
    //registerLabel{ nullptr },
    //userNameLogin{ nullptr },
    //loginOpt{ nullptr }
{
    ui.setupUi(this);


    //userNameLogin = new WUserNameLogin{ this };
    //mobileLogin = new WMobileLogin{ this };
    //loginOpt = new WLoginOpt{ this };

    TitleChanged(WLoginSwitch::ETitle::userName);

    //registerLabel = loginOpt->registerLabel;
    //loginBtn = loginOpt->loginBtn;

    connect(ui.btLanguageText, &QPushButton::clicked, this, &WLogin::LanguageClicked);
    connect(ui.loginSwitch, &WLoginSwitch::TitleChanged, this, &WLogin::TitleChanged);
}

WLogin::~WLogin()
{

}

bool  WLogin::remberPwd()
{
    //if (_cur_login_title == WLoginSwitch::ETitle::userName) {
    //    return userNameLogin->isRember();
    //}
    return false;
}

QString WLogin::UserName()
{
    return "";// userNameLogin->userNameEdit->text();
}

QString WLogin::Password()
{
    return "";// userNameLogin->passwordEdit->text();
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

        //mobileLogin->hide();
        //userNameLogin->move(0, 296);
        //loginOpt->move(0, 520);
        //userNameLogin->show();
    }
    else
    {
        ui.stackedWidget->setCurrentIndex(1);

        //userNameLogin->hide();
        //mobileLogin->move(0, 296);
        //loginOpt->move(0, 621);
        //mobileLogin->show();
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



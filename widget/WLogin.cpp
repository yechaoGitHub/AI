#include "WLogin.h"

WLogin::WLogin(QWidget* parent) :
    QWidget{ parent },
    registerLabel{ nullptr },
    userNameLogin{ nullptr },
    loginOpt{ nullptr }
{
    ui.setupUi(this);

    userNameLogin = new WUserNameLogin{ this };
    mobileLogin = new WMobileLogin{ this };
    loginOpt = new WLoginOpt{ this };
    mobileLogin->hide();

    userNameLogin->move(0, 336);
    loginOpt->move(0, 532);

    registerLabel = loginOpt->registerLabel;
    loginBtn = loginOpt->loginBtn;

    connect(ui.loginSwitch, &WLoginSwitch::TitleChanged, this, &WLogin::TitleChanged);
}

WLogin::~WLogin()
{

}

QString WLogin::UserName()
{
    return userNameLogin->userNameEdit->text();
}

QString WLogin::Password()
{
    return userNameLogin->passwordEdit->text();
}

void WLogin::TitleChanged(WLoginSwitch::ETitle title)
{
    if (title == WLoginSwitch::ETitle::userName) 
    {
        mobileLogin->hide();
        userNameLogin->move(0, 336);
        loginOpt->move(0, 532);
        userNameLogin->show();
    }
    else 
    {
        userNameLogin->hide();
        mobileLogin->move(0, 336);
        loginOpt->move(0, 661);
        mobileLogin->show();
    }
}



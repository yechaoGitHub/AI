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

    TitleChanged(WLoginSwitch::ETitle::userName);

    registerLabel = loginOpt->registerLabel;
    loginBtn = loginOpt->loginBtn;

    connect(ui.loginSwitch, &WLoginSwitch::TitleChanged, this, &WLogin::TitleChanged);

    /*QString username; QString pwd;
    connect(loginBtn, &WButton::clicked, this, [&] {
        if()
        userNameLogin->GetLoginInfo(username,pwd);
        });*/
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
    _cur_login_title = title;
    if (title == WLoginSwitch::ETitle::userName) 
    {
        mobileLogin->hide();
        userNameLogin->move(0, 296);
        loginOpt->move(0, 520);
        userNameLogin->show();
    }
    else 
    {
        userNameLogin->hide();
        mobileLogin->move(0, 296);
        loginOpt->move(0, 621);
        mobileLogin->show();
    }
}



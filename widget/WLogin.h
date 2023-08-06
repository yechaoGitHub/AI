#pragma once

#include <QWidget>
#include "ui_login.h"
#include "WUserNameLogin.h"
#include "WMobileLogin.h"
#include "WLoginOpt.h"


class WLogin : public QWidget
{
    Q_OBJECT
public:
    WLogin(QWidget* parent = nullptr);
    ~WLogin();

    QString UserName();
    QString Password();
    bool    remberPwd();

    WClickedLabel*  registerLabel;
    WButton*        loginBtn;

private:
    void TitleChanged(WLoginSwitch::ETitle title);

    Ui::Login       ui;
    WUserNameLogin* userNameLogin;
    WMobileLogin*   mobileLogin;
    WLoginOpt*      loginOpt;

    WLoginSwitch::ETitle  _cur_login_title;
};


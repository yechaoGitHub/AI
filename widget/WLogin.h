#pragma once

#include <QWidget>
#include <QCheckBox>
#include "ui_login.h"
#include "WUserNameLogin.h"
#include "WMobileLogin.h"

class WLogin : public QWidget
{
    Q_OBJECT
public:
    WLogin(QWidget* parent = nullptr);
    ~WLogin();

    QString UserName();
    QString Password();
    bool remberPwd();

    QPushButton*        registerLabel = nullptr;
    QPushButton*        loginBtn = nullptr;
    QPushButton*        forgotPasswordBtn = nullptr;;
    QCheckBox*          cbRemeber = nullptr;

protected:
    void changeEvent(QEvent* event) override;

private:
    void TitleChanged(WLoginSwitch::ETitle title);
    void LoginClicked();
    void LanguageClicked();
    void PasswordLogin();
    void EmailLogin();
    void PhoneLogin();

    Ui::Login               ui;
    WLoginSwitch::ETitle    _cur_login_title;
};


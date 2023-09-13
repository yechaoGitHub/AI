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

    QPushButton*        registerLabel;
    QPushButton*        loginBtn;
    QCheckBox*          cbRemeber;

protected:
    void changeEvent(QEvent* event) override;

private:
    void TitleChanged(WLoginSwitch::ETitle title);

    void LanguageClicked();

    Ui::Login       ui;

    WLoginSwitch::ETitle  _cur_login_title;
};


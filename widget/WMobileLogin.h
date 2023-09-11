#pragma once
#include <QWidget>
#include "ui_mobileLogin.h"

class WMobileLogin : public QWidget
{
    Q_OBJECT
public:
    WMobileLogin(QWidget* parent = nullptr);
    ~WMobileLogin();

    //WCheckBox* loggedCheckBox;

    QString PhoneNumber();
    QString VerifyCode();

private:
    void GetCodeCallback();

    Ui::mobileLogin ui;
};


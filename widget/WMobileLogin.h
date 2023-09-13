#pragma once
#include <QWidget>
#include "ui_mobileLogin.h"

class WMobileLogin : public QWidget
{
    Q_OBJECT
public:
    WMobileLogin(QWidget* parent = nullptr);
    ~WMobileLogin();

    QString PhoneNumber();
    QString VerifyCode();

protected:
    void showEvent(QShowEvent* event) override;

private:
    void GetCodeCallback();

    Ui::mobileLogin ui;
};


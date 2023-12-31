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
    QString DialingCode();
    QString PhoneCode();

protected:
    void timerEvent(QTimerEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;
private:
    void GetCodeCallback();
    void CountryChanged(int index);
    void ReloadPhoneNumer();

    QString _selectCode;
    Ui::mobileLogin ui;
    bool _canResend = true;
    int _downCount = 60;
    int _timer = 0;
};


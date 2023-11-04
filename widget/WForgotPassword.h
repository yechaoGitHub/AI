#pragma once
#include <QWidget>
#include <QString>
#include <QEvent >
#include "ui_forgotPassword.h"

class WForgotPassword : public QWidget
{
public:
    WForgotPassword(QWidget* parent = nullptr);
    ~WForgotPassword();

    QString DialingCode();
    QString UserName();
    QString Password();
    QString Repassword();
    QString VerifyCode();
    void Clear();

    QPushButton* verifyBtn;

protected:
    void showEvent(QShowEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    void GetVCodeClicked();
    void CountryChanged(int index);

    Ui::ForgotPassword  ui;
    int                 _timer = 0;
    int                 _downCount = 0;
};

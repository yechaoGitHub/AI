#pragma once
#include <QWidget>
#include <QString>
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

    QPushButton* verifyBtn;

protected:
    void showEvent(QShowEvent* event) override;

private:
    void GetVCodeClicked();

    Ui::ForgotPassword ui;
};

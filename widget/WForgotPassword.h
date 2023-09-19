#pragma once
#include <QWidget>
#include "ui_forgotPassword.h"

class WForgotPassword : public QWidget
{
public:
    WForgotPassword(QWidget* parent = nullptr);
    ~WForgotPassword();

    QPushButton* verifyBtn;

private:
    Ui::ForgotPassword ui;

};

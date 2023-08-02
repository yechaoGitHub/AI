#pragma once
#include <QWidget>
#include <ui_userNameLogin.h>

class WUserNameLogin : public QWidget
{
    Q_OBJECT
public:
    WUserNameLogin(QWidget* parent = nullptr);
    ~WUserNameLogin();

    QLineEdit* userNameEdit;
    QLineEdit* passwordEdit;

private:
    Ui::userNameLogin ui;
};


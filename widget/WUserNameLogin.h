#pragma once
#include <QWidget>
#include <ui_userNameLogin.h>

class WUserNameLogin : public QWidget
{
    Q_OBJECT
public:
    WUserNameLogin(QWidget* parent = nullptr);
    ~WUserNameLogin();

    QLineEdit*          userNameEdit = nullptr;
    QLineEdit*          passwordEdit = nullptr;

    //bool    isRember();
protected:
    void changeEvent(QEvent* event) override;
private:
    Ui::userNameLogin ui;
};


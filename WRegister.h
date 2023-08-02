#pragma once
#include <QWidget.h>
#include "ui_signUp.h"

class WRegister : public QWidget
{
    Q_OBJECT
public:
    WRegister(QWidget* parent = nullptr);
    ~WRegister();

    QString UserName();
    QString Password();
    QString PhoneNumber();
    QString VerifyCode();

    bool Check();

    WButton* signBtn;

private:
    Ui::signUp ui;
};


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

    QPushButton* signBtn;

protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::signUp ui;
};


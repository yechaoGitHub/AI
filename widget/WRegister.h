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
    QString Repassword();
    QString VerifyCode();
    QString DialingCode();
    QString VerfyCode();
    QString RecommondCode();

    QPushButton* signBtn;

protected:
    void showEvent(QShowEvent* event) override;

private:
    void GetCodeCallback();

    Ui::signUp ui;
};


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
    void Clear();

    QPushButton* signBtn;

protected:
    void showEvent(QShowEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    void GetCodeCallback();
    void CountryChanged(int index);

    Ui::signUp      ui;
    int             _timer = 0;
    int             _downCount = 0;
};


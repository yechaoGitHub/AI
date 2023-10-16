#pragma once

#include <QWidget>
#include "ui_WLoginUI.h"
#include "model/FrameLessWidget.h"


class WLoginUI : public FrameLessWidget
{
    Q_OBJECT

public:
    WLoginUI(QWidget *parent = nullptr);
    ~WLoginUI();

private:
    void BackClicked();
    void ForgetPasswordClicked();
    void SignUpClicked();
    void RegisterClicked();
    void ForgotClicked();
    void ClearPage();

    Ui::WLoginUIClass ui;
};

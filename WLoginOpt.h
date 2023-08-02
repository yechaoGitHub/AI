#pragma once
#include <QWidget>
#include "WClickedLabel.h"
#include "ui_loginOpt.h"

class WLoginOpt : public QWidget
{
    Q_OBJECT

public:
    WLoginOpt(QWidget* parent = nullptr);
    ~WLoginOpt();

    WClickedLabel*  registerLabel;
    WButton*        loginBtn;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::loginOpt    ui;
};


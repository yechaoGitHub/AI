#pragma once
#include <QWidget>
#include <QDialog>
#include "ui_saveTip.h"

class WSaveTip : public QDialog
{
public:
    WSaveTip(QWidget* parent = nullptr);
    ~WSaveTip();

    bool Sure();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void CloseClicked();
    void ConfirmClicked();
    void CancelClicked();

    Ui::saveTip ui;

    bool _sure = false;
};


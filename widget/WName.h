#pragma once
#include <QDialog>
#include "ui_name.h"

class WName : public QDialog
{
public:
    WName(QWidget* parent = nullptr);
    ~WName();

    QString Name();
    bool Confirmed();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void CloseClicked();
    void ConfirmClicked();

    Ui::name            ui;
    bool                _confirmed = false;
    QString             _name;
};

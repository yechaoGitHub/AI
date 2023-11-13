#pragma once

#include <QWidget>
#include "ui_WRobotChatMainUI.h"
#include "../model/FrameLessWidget.h"


class RobotChatMainUI : public FrameLessWidget
{
    Q_OBJECT

public:
    RobotChatMainUI(QWidget *parent = nullptr);
    ~RobotChatMainUI();

    void Clear();
    void Flush();
    void Show(bool record = false);
    void ShowRecord(const QString& chatId);

protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

private slots:
    void on_pb_min_clicked();
    void on_pb_close_clicked();

private:
    void SaveBtnClicked();
    void ShowMessage(QString msg);

    Ui::RobotChatMainUIClass    ui;
    int                         _saveCounter = 0;
};

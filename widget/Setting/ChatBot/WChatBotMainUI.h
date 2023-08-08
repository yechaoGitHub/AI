#pragma once

#include <QWidget>
#include "ui_WChatBotMainUI.h"

class WChatBotMainUI : public QWidget
{
	Q_OBJECT

public:
	WChatBotMainUI(QWidget *parent = nullptr);
	~WChatBotMainUI();

private:
	void changeSelectBtn(WNavbarButton::BarType type);

private slots:
	void on_pb_lib_clicked();
	void on_pb_history_clicked();

private:
	Ui::WChatBotMainUIClass ui;
};

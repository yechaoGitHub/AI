#pragma once

#include <QWidget>
#include "ui_WRobotNavigation.h"
#include "model/FrameLessWidget.h"
#include "WRobotSettingMenu.h"


enum Navig_Type {
	Chat = 0,
	Voice,
	Speech,
	System_Set,
	Chat_Set,
	Quite
};

class WRobotNavigation : public FrameLessWidget
{
	Q_OBJECT
signals:
	void sig_robot_clicked(Navig_Type type);
public:
	WRobotNavigation(QWidget *parent = nullptr);
	~WRobotNavigation();

protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* e);
private slots:
	void on_pb_chat_clicked();
	void on_pb_voice_clicked();
	void on_pb_speech_clicked();

private:
	Ui::WRobotNavigationClass ui;
	WRobotSettingMenu*		_setting_menu = nullptr;
};

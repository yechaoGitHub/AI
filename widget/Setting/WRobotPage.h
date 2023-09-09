#pragma once

#include <QWidget>
#include "ui_WRobotPage.h"


class WRobotPage : public QWidget
{
	Q_OBJECT
	signals:
		void sig_robot_clicked(bool);
public:
	WRobotPage(QWidget *parent = nullptr);
	~WRobotPage();

	void initCheck();
	void initUI();
private slots:
	void on_pb_robot_lock_clicked();
	void on_pb_sound_lock_clicked();
	void on_pb_robot_setting_clicked();
	void on_pb_sound_set_clicked();
private:
	Ui::WRobotPageClass ui;

	bool	_robot_open = true;
	bool	_sound_open = true;
};

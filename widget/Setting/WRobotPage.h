#pragma once

#include <QWidget>
#include "ui_WRobotPage.h"

class WRobotPage : public QWidget
{
	Q_OBJECT

public:
	WRobotPage(QWidget *parent = nullptr);
	~WRobotPage();

private slots:
	void on_pb_robot_lock_clicked();
	void on_pb_sound_lock_clicked();
private:
	Ui::WRobotPageClass ui;

	bool	_robot_open = true;
	bool	_sound_open = true;
};

#pragma once

#include <QWidget>
#include "ui_WRobotSettingMenu.h"

class WRobotSettingMenu : public QWidget
{
	Q_OBJECT
signals:
	void sig_settingMenu_clicked(int);
public:
	WRobotSettingMenu(QWidget *parent = nullptr);
	~WRobotSettingMenu();

	void	setFloatText(const QString& text);
protected:
	void changeEvent(QEvent* event) override;
private:
	Ui::WRobotSettingMenuClass ui;
};

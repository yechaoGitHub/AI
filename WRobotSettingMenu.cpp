#include "WRobotSettingMenu.h"

WRobotSettingMenu::WRobotSettingMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pb_system, &QPushButton::clicked, this, [=] {
		emit sig_settingMenu_clicked(1);
		});
	connect(ui.pb_chatbot, &QPushButton::clicked, this, [=] {
		emit sig_settingMenu_clicked(2);
		});
	connect(ui.pb_quite, &QPushButton::clicked, this, [=] {
		emit sig_settingMenu_clicked(3);
		});
}

void WRobotSettingMenu::setFloatText(const QString& text)
{
	ui.pb_quite->setText(text);
}

WRobotSettingMenu::~WRobotSettingMenu()
{}

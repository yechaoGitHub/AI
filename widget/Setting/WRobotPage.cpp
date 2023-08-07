#include "WRobotPage.h"
#include <qstyle.h>


WRobotPage::WRobotPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground);
	ui.frame_chat->setProperty("is_sel",true);
	ui.frame_chat->style()->polish(ui.frame_chat);

	ui.frame_sound->setProperty("is_sel", true);
	ui.frame_sound->style()->polish(ui.frame_sound);

	ui.pb_robot_lock->setProperty("open", true);
	ui.pb_robot_lock->style()->polish(ui.pb_robot_lock);

	ui.pb_sound_lock->setProperty("open", true);
	ui.pb_sound_lock->style()->polish(ui.pb_sound_lock);
}

WRobotPage::~WRobotPage()
{}

void WRobotPage::on_pb_robot_lock_clicked()
{
	_robot_open = !_robot_open;
	ui.frame_chat->setProperty("is_sel", _robot_open);
	ui.frame_chat->style()->polish(ui.frame_chat);

	ui.pb_robot_lock->setProperty("open", _robot_open);
	ui.pb_robot_lock->style()->polish(ui.pb_robot_lock);

}

void WRobotPage::on_pb_sound_lock_clicked()
{
	_sound_open = !_sound_open;

	ui.frame_sound->setProperty("is_sel", _sound_open);
	ui.frame_sound->style()->polish(ui.frame_sound);

	ui.pb_sound_lock->setProperty("open", _sound_open);
	ui.pb_sound_lock->style()->polish(ui.pb_sound_lock);
}
#include "WRobotPage.h"
#include <qstyle.h>
#include "base/GlobalSetting.h"


WRobotPage::WRobotPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground);

	_robot_open = SETTING.getRobotBot();
	_sound_open = SETTING.getSoundBot();
	ui.pb_robot_lock->setChecked(SETTING.getRobotBot());
	ui.pb_sound_lock->setChecked(SETTING.getSoundBot());

	ui.lb_robot_name->setText(tr("Integrate a variety of the latest and best large language models to achieve dialogue, question answering, copy generation and so on. The most efficient tool and encyclopedia assistant!"));
	ui.lb_robot_name->setWordWrap(true);
	ui.lb_sound_tip->setText(tr("Support more than a dozen commonly used languages real-time translation, speech synthesis. Talk to the world in your own language!"));
	ui.lb_sound_tip->setWordWrap(true);
	initUI();
}

WRobotPage::~WRobotPage()
{}

void WRobotPage::on_pb_robot_lock_clicked()
{
	_robot_open = ui.pb_robot_lock->isChecked();
	SETTING.setRobotBot(_robot_open);
	initUI();
}

void WRobotPage::on_pb_sound_lock_clicked()
{
	_sound_open = ui.pb_sound_lock->isChecked();
	SETTING.setRobotBot(_sound_open);
	initUI();
}

void WRobotPage::initUI()
{
	ui.frame_chat->setProperty("is_sel", _robot_open);
	ui.frame_chat->style()->polish(ui.frame_chat);

	ui.frame_sound->setProperty("is_sel", _sound_open);
	ui.frame_sound->style()->polish(ui.frame_sound);

	ui.lb_aced->setVisible(_robot_open);
	ui.lb_sound_act->setVisible(_sound_open);
}

void WRobotPage::on_pb_robot_setting_clicked()
{
	emit sig_robot_clicked(true);
}

void WRobotPage::on_pb_sound_set_clicked()
{
	emit sig_robot_clicked(false);
}
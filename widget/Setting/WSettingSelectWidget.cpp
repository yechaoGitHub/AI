#include "WSettingSelectWidget.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"


WSettingSelectWidget::WSettingSelectWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.pb_account->initBar("Account", WNavbarButton::Bar_Account);
	changeSelectBtn(WNavbarButton::Bar_Account);
	ui.pb_help->initBar("Robot", WNavbarButton::Bar_Help);
	ui.pb_team->initBar("Team", WNavbarButton::Bar_Team);
	ui.pb_tools->initBar("General Settings", WNavbarButton::Bar_Tools);

	connect(ui.robot_page, &WRobotPage::sig_robot_clicked, this, [=](int is_robot) {
		emit sig_robot_clicked(is_robot);
		});
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_getUserInfoReplay,this, &WSettingSelectWidget::slot_getUserInfoReplay);
}

WSettingSelectWidget::~WSettingSelectWidget()
{}

void WSettingSelectWidget::initData()
{
	SettingInterfaceBussiness::getInstance()->getUserInfoReq();
}

void WSettingSelectWidget::slot_getUserInfoReplay(bool success, int code, const QString& msg, const stru_UserInfo& user_info)
{
	if (success && code == netCode::Success) {
		ui.account_widget->initAccount(user_info);
	}
}

void WSettingSelectWidget::on_pb_account_clicked()
{
	emit sig_page_change(false);
	changeSelectBtn(WNavbarButton::Bar_Account);
	ui.stackedWidget->setCurrentWidget(ui.account_widget);
}

void WSettingSelectWidget::on_pb_help_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Help);
	emit sig_page_change(false);
	ui.stackedWidget->setCurrentWidget(ui.robot_page);
}

void WSettingSelectWidget::on_pb_team_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Team);
	emit sig_page_change(true);
	ui.stackedWidget->setCurrentWidget(ui.invite_page);
	static bool first = true;
	if (first) {
		first = false;
		ui.invite_page->reqTeamData();
	}
}

void WSettingSelectWidget::on_pb_tools_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Tools);
	emit sig_page_change(true);
	ui.stackedWidget->setCurrentWidget(ui.gerenal_page);
}

void WSettingSelectWidget::changeSelectBtn(WNavbarButton::BarType type)
{
	ui.pb_account->setSelect(type == WNavbarButton::BarType::Bar_Account);
	ui.pb_help->setSelect(type == WNavbarButton::BarType::Bar_Help);
	ui.pb_team->setSelect(type == WNavbarButton::BarType::Bar_Team);
	ui.pb_tools->setSelect(type == WNavbarButton::BarType::Bar_Tools);
}
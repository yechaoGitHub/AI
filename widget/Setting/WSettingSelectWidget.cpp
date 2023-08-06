#include "WSettingSelectWidget.h"

WSettingSelectWidget::WSettingSelectWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.pb_account->initBar("Account", WNavbarButton::Bar_Account);
	changeSelectBtn(WNavbarButton::Bar_Account);
	ui.pb_help->initBar("Robot", WNavbarButton::Bar_Help);
	ui.pb_team->initBar("Team", WNavbarButton::Bar_Team);
	ui.pb_tools->initBar("General Settings", WNavbarButton::Bar_Tools);

}

WSettingSelectWidget::~WSettingSelectWidget()
{}

void WSettingSelectWidget::on_pb_account_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Account);
	ui.stackedWidget->setCurrentWidget(ui.account_widget);
}

void WSettingSelectWidget::on_pb_help_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Help);
}

void WSettingSelectWidget::on_pb_team_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Team);
}

void WSettingSelectWidget::on_pb_tools_clicked()
{
	changeSelectBtn(WNavbarButton::Bar_Tools);
	ui.stackedWidget->setCurrentWidget(ui.gerenal_page);
}

void WSettingSelectWidget::changeSelectBtn(WNavbarButton::BarType type)
{
	ui.pb_account->setSelect(type == WNavbarButton::BarType::Bar_Account);
	ui.pb_help->setSelect(type == WNavbarButton::BarType::Bar_Help);
	ui.pb_team->setSelect(type == WNavbarButton::BarType::Bar_Team);
	ui.pb_tools->setSelect(type == WNavbarButton::BarType::Bar_Tools);
}
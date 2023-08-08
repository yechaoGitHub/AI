#include "WChatBotMainUI.h"
#include <qstyle.h>


WChatBotMainUI::WChatBotMainUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pb_lib->initBar("Libarary", WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->initBar("History", WNavbarButton::BarType::Bar_History);
	changeSelectBtn(WNavbarButton::BarType::Bar_Lib);

	ui.pb_open->setProperty("open", true);
	ui.pb_open->style()->polish(ui.pb_open);
}

WChatBotMainUI::~WChatBotMainUI()
{}

void WChatBotMainUI::on_pb_lib_clicked()
{

}

void WChatBotMainUI::on_pb_history_clicked()
{

}

void WChatBotMainUI::changeSelectBtn(WNavbarButton::BarType type)
{
	ui.pb_lib->setSelect(type == WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->setSelect(type == WNavbarButton::BarType::Bar_History);
}
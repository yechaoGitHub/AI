#include "WChatBotMainUI.h"
#include <qstyle.h>
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include <QTimer>


WChatBotMainUI::WChatBotMainUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pb_lib->initBar("Libarary", WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->initBar("History", WNavbarButton::BarType::Bar_History);
	changeSelectBtn(WNavbarButton::BarType::Bar_Lib);

	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_chatBotListReplay, this, &WChatBotMainUI::slot_getChatBotType);

	WLabelButton* label_btn = new WLabelButton(this);
	_pre_select_btn = label_btn;
	label_btn->setType(-1,tr("All templates"));
	label_btn->setSelected(true);
	ui.horizontalLayout_2->addWidget(label_btn);
	connect(label_btn, &QPushButton::clicked, this, &WChatBotMainUI::slot_type_btn_clicked);
}

WChatBotMainUI::~WChatBotMainUI()
{}

void WChatBotMainUI::slot_getChatBotType(bool, int, const QMap<int, QString>& type_map)
{
	for (auto& key : type_map.keys()) {
		WLabelButton* label_btn = new WLabelButton(this);
		label_btn->setType(key, type_map.value(key));
		ui.horizontalLayout_2->addWidget(label_btn);
		connect(label_btn, &QPushButton::clicked, this, &WChatBotMainUI::slot_type_btn_clicked);
	}
	//ui.horizontalLayout_2->addStretch();
}

void WChatBotMainUI::slot_type_btn_clicked()
{
	WLabelButton* btn = (WLabelButton*)QObject::sender();
	if (btn && _pre_select_btn!= btn) {
		btn->setSelected(true);
	}
	if (_pre_select_btn) {
		_pre_select_btn->setSelected(false);
	}
	_pre_select_btn = btn;
	ui.lib_page->updateLibBySelType(btn->getType());
}

void WChatBotMainUI::on_pb_lib_clicked()
{
	changeSelectBtn(WNavbarButton::BarType::Bar_Lib);
	ui.stackedWidget->setCurrentWidget(ui.lib_page);
	ui.frame_2->setVisible(true);
}

void WChatBotMainUI::slot_back()
{
	emit sig_chatBotBack();
}

void WChatBotMainUI::on_pb_history_clicked()
{
	changeSelectBtn(WNavbarButton::BarType::Bar_History);
	ui.stackedWidget->setCurrentWidget(ui.history_page);
	ui.frame_2->setVisible(false);
}

void WChatBotMainUI::changeSelectBtn(WNavbarButton::BarType type)
{
	ui.pb_lib->setSelect(type == WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->setSelect(type == WNavbarButton::BarType::Bar_History);

	static bool first = true;
	static bool first_chatbot = true;
	static bool first_sound = true;
	if (!first) {
		if (first_chatbot && type == WNavbarButton::BarType::Bar_Lib) {
			first_chatbot = false;
			ui.lib_page->getChatBotTemplate();
			SettingInterfaceBussiness::getInstance()->getChatBotType();
		}
		else if (first_sound && type == WNavbarButton::BarType::Bar_History) {
			first_sound = false;
			ui.history_page->reqChatHistory();
		}
	}
	else {
		first = false;
	}
}
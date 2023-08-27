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

	qRegisterMetaType<QVector<strc_ChatbotInfo>>("QVector<strc_ChatbotInfo>");
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_getChatBotListReplay,this, &WChatBotMainUI::slot_getChatBotListReplay);
	for (int var = 0; var < 5; var++) {
		WLabelButton* label_btn = new WLabelButton(this);
		label_btn->setType(var, QString("test001----%1").arg(var));
		ui.horizontalLayout_2->addWidget(label_btn);
		connect(label_btn,&QPushButton::clicked,this,&WChatBotMainUI::slot_type_btn_clicked);
	}
	ui.horizontalLayout_2->addStretch();
}

WChatBotMainUI::~WChatBotMainUI()
{}

void WChatBotMainUI::slot_getChatBotListReplay(bool success, int, const QString& msg, const QVector<strc_ChatbotInfo>& chatbot_list)
{
	if (success) {
		for (auto it : chatbot_list) {
			auto item = std::find_if(_chatBot_type_list.begin(), _chatBot_type_list.end(), [=](int type) {
				return it.type == type;
				});

			if (item == _chatBot_type_list.end()) {
				_chatBot_type_list.push_back(it.type);
				WLabelButton* label_btn = new WLabelButton(this);
				label_btn->setType(it.type,it.typeName);
				ui.horizontalLayout_2->addWidget(label_btn);
				_chatBot_btn_list.push_back(label_btn);
				connect(label_btn, &QPushButton::clicked, this, &WChatBotMainUI::slot_type_btn_clicked);
			}
		}
		ui.lib_page->updateLib(chatbot_list);
		ui.horizontalLayout_2->addStretch();
	}
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

}

void WChatBotMainUI::on_pb_lib_clicked()
{
	changeSelectBtn(WNavbarButton::BarType::Bar_Lib);
	ui.stackedWidget->setCurrentWidget(ui.lib_page);
}

void WChatBotMainUI::slot_back()
{
	emit sig_chatBotBack();
}

void WChatBotMainUI::on_pb_history_clicked()
{
	changeSelectBtn(WNavbarButton::BarType::Bar_History);
	ui.stackedWidget->setCurrentWidget(ui.history_page);
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
			QTimer::singleShot(200, this, [=]() {
				SettingInterfaceBussiness::getInstance()->getCharBotListReq();
				});

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
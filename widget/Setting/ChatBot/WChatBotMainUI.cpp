#include "WChatBotMainUI.h"
#include <qstyle.h>
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include <QTimer>
#include "base/GlobalSetting.h"


WChatBotMainUI::WChatBotMainUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pb_lib->initBar(tr("Library"), WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->initBar(tr("History"), WNavbarButton::BarType::Bar_History);
	changeSelectBtn(WNavbarButton::BarType::Bar_Lib);

	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_chatBotListReplay, this, &WChatBotMainUI::slot_getChatBotType);

	WLabelButton* label_btn = new WLabelButton(this);
	_pre_select_btn = label_btn;
	label_btn->setType(-1,tr("All templates"));
	label_btn->setSelected(true);
	ui.horizontalLayout_2->addWidget(label_btn);
	connect(label_btn, &QPushButton::clicked, this, &WChatBotMainUI::slot_type_btn_clicked);

	connect(ui.pb_open, &QCheckBox::clicked, this, [=] {
		if (ui.pb_open->checkState() == Qt::CheckState::Checked) {
			SETTING.setRobotBot(true);
		}
		else {
			SETTING.setRobotBot(false);
		}
		});

	ui.label->setVisible(false);
	ui.pb_open->setVisible(false);
}

WChatBotMainUI::~WChatBotMainUI()
{}

void WChatBotMainUI::initCheck()
{
	ui.pb_open->setChecked(SETTING.getRobotBot());
	ui.pb_lib->click();
}

void WChatBotMainUI::slot_getChatBotType(bool, int, const QMap<int, QString>& type_map)
{
	for (auto& key : type_map.keys()) {
		WLabelButton* label_btn = new WLabelButton(this);
		label_btn->setType(key, type_map.value(key));
		ui.horizontalLayout_2->addWidget(label_btn);
		connect(label_btn, &QPushButton::clicked, this, &WChatBotMainUI::slot_type_btn_clicked);
	}
	ui.horizontalLayout_2->addStretch();
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
	_cur_type = WNavbarButton::BarType::Bar_Lib;
	ui.pb_lib->setSelect(type == WNavbarButton::BarType::Bar_Lib);
	ui.pb_history->setSelect(type == WNavbarButton::BarType::Bar_History);

	if (type == WNavbarButton::BarType::Bar_Lib) {
		ui.lib_page->getChatBotTemplate();
		static int first_bot = 0;
		first_bot++;
		if (first_bot == 2) {
			SettingInterfaceBussiness::getInstance()->getChatBotType();
		}
	}
	else if (type == WNavbarButton::BarType::Bar_History) {
		ui.history_page->reqChatHistory();
	}
}

void WChatBotMainUI::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}
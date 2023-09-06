#include "WLibarary.h"
#include "Bussiness/SettingInterfaceBussiness.h"


WLibarary::WLibarary(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			WLibModelWidget* lib = new WLibModelWidget(this);
			connect(lib, &WLibModelWidget::sig_libModel_click,this,&WLibarary::slot_model_clicked);
			_lib_model_list.push_back(lib);
			ui.gridLayout->addWidget(lib, i,j);
		}
	}
	connect(ui.widget,&WPageCtlWidget::sig_changePage,this, &WLibarary::slot_page_change);

	qRegisterMetaType<QVector<strc_ChatbotInfo>>("QVector<strc_ChatbotInfo>");
	qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_getChatBotListReplay, this, &WLibarary::slot_getChatBotListReplay);
}

WLibarary::~WLibarary()
{}

void WLibarary::getChatBotTemplate()
{
	SettingInterfaceBussiness::getInstance()->getCharBotListReq(_cur_page, _page_size, _cur_type);
}

void WLibarary::updateLibBySelType(int type)
{
	_cur_type = type;
	getChatBotTemplate();
}

void WLibarary::slot_model_clicked()
{

}

void WLibarary::slot_page_change(int index)
{
	SettingInterfaceBussiness::getInstance()->getCharBotListReq(index, _page_size, _cur_type);
}

void WLibarary::slot_getChatBotListReplay(bool success, int, const strc_PageInfo& page_info, const QVector<strc_ChatbotInfo>& chatbot_list)
{
	if (success) {
		int list_index = 0;
		for (auto it : chatbot_list) {
			_lib_model_list.at(list_index)->setTitle(tr("Chatbot(%1)").arg(it.name), it.desc);
			_lib_model_list.at(list_index)->show();

			list_index++;
		}

		for (auto i = list_index; i < _page_size; i++) {
			_lib_model_list.at(i)->hide();
		}

		_total_size = page_info.total_size;
		_pages = page_info.total_pages;
		_cur_page = page_info.cur_page;
		ui.widget->initCtl(_pages, _total_size, _cur_page);
	}
}

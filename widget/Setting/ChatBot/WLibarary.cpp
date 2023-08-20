#include "WLibarary.h"

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
		ui.gridLayout->setRowStretch(i,400);
	}
	connect(ui.widget,&WPageCtlWidget::sig_changePage,this, &WLibarary::slot_page_change);
}

WLibarary::~WLibarary()
{}

void WLibarary::slot_model_clicked()
{

}

void WLibarary::slot_page_change(int index)
{
	_cur_page = index;
	int start_index = (_cur_page - 1) * 6;
	int list_index = 0;
	for (int var = start_index; var < _chatBot__type_list.size() - start_index; var++) {
		if (list_index > 5) {
			break;
		}
		_lib_model_list.at(list_index)->setTitle(tr("Chatbot(%1)").arg(_chatBot__type_list.at(list_index).name), _chatBot__type_list.at(list_index).desc);
		_lib_model_list.at(list_index)->show();
		list_index++;
	}

	for (auto i = list_index; i < 6; i++) {
		_lib_model_list.at(i)->hide();
	}
}

void WLibarary::updateLib(const QVector<strc_ChatbotInfo>& chatbot_list)
{
	_chatBot_list = chatbot_list;
	updateLibBySelType(-1);
}

void WLibarary::updateLibBySelType(int type)
{
	_cur_type = type;
	_chatBot__type_list.clear();
	if (type == -1) {//all
		_chatBot__type_list = _chatBot_list;
	}
	else {

		for (auto it : _chatBot_list) {
			if (it.type == type) {
				_chatBot__type_list.push_back(it);
			}
		}

	}

	_total_size = _chatBot__type_list.size();
	_pages = ceil(_total_size / 6);
	_cur_page = 1;
	ui.widget->initCtl(_pages, _total_size, _cur_page);
	slot_page_change(0);
}
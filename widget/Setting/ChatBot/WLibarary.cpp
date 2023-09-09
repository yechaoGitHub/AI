#include "WLibarary.h"
#include "Bussiness/SettingInterfaceBussiness.h"
#include "base/GlobalSetting.h"


WLibarary::WLibarary(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			WLibModelWidget* lib = new WLibModelWidget(this);
			QSizePolicy sp_retain = lib->sizePolicy();
			sp_retain.setRetainSizeWhenHidden(true);
			lib->setSizePolicy(sp_retain);
			connect(lib, &WLibModelWidget::sig_libModel_click,this,&WLibarary::slot_model_clicked);
			_lib_model_list.push_back(lib);
			ui.gridLayout->addWidget(lib, i,j);
		}
	}
	_select_model = _lib_model_list.at(0);
	connect(ui.widget,&WPageCtlWidget::sig_changePage,this, &WLibarary::slot_page_change);

	qRegisterMetaType<QVector<strc_ChatbotInfo>>("QVector<strc_ChatbotInfo>");
	qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_getChatBotListReplay, this, &WLibarary::slot_getChatBotListReplay);
	QString model = SETTING.getRebotModel();
	if (!model.isEmpty()) {
		_model_list = model.split(";");
	}
}

WLibarary::~WLibarary()
{}

void WLibarary::getChatBotTemplate()
{
	SettingInterfaceBussiness::getInstance()->getCharBotListReq(_cur_page, _page_size, _cur_type);
}

void WLibarary::setModelOpen(bool open)
{
	if (_select_model) {
		_select_model->setSel(open);
		int id = _select_model->getModelId();
		opeModelswitch(open,id);
	}
}

void WLibarary::updateLibBySelType(int type)
{
	_cur_type = type;
	getChatBotTemplate();
}

void WLibarary::slot_model_clicked()
{
	_select_model = static_cast<WLibModelWidget*>(QObject::sender());
	if (_select_model) {
		emit sig_model_sel(_select_model->is_sel());
	}
}

void WLibarary::slot_page_change(int index)
{
	SettingInterfaceBussiness::getInstance()->getCharBotListReq(index, _page_size, _cur_type);
}

void WLibarary::opeModelswitch(bool open,int id)
{
	QString strList;
	bool exist = false;
	for (auto it : _model_list) {
		if (it == QString::number(id)) {
			exist = true;
			if (!open) {
				continue;
			}
		}
		strList = strList.append(it).append(";");
	}

	if (open && !exist) {
		strList = strList.append(QString::number(id));
		_model_list.push_back(QString::number(id));
	}

	if (!open && exist) {
		_model_list.removeAll(QString::number(id));
	}

	SETTING.setRebotModel(strList);
}

bool WLibarary::hasOpen(int id)
{
	for (auto it : _model_list) {
		if (it == QString::number(id)) {
			return  true;
		}
	}

	return false;
}

void WLibarary::slot_getChatBotListReplay(bool success, int, const strc_PageInfo& page_info, const QVector<strc_ChatbotInfo>& chatbot_list)
{
	if (success) {
		int list_index = 0;
		for (auto it : chatbot_list) {
			_lib_model_list.at(list_index)->setTitle(tr("Chatbot(%1)").arg(it.name), it.desc,it.id);
			_lib_model_list.at(list_index)->show();

			_lib_model_list.at(list_index)->setSel(hasOpen(it.id));
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

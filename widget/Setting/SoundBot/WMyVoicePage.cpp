#include "WMyVoicePage.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/AiSound.h"
#include <QDesktopServices>


WMyVoicePage::WMyVoicePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->setFixedSize(660,572);
	_voice_widget_list.push_back(ui.widget_2);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			WMyVoiceModel* voice = new WMyVoiceModel(this);
			ui.gridLayout->addWidget(voice,i,j);
			_voice_widget_list.push_back(voice);
		}
	}

	qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
	qRegisterMetaType<QVector<strc_MyVoice>>("QVector<strc_MyVoice>");
	connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_common_replay,this, &WMyVoicePage::slot_commonReplay);
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_myVoiceListReplay, this, &WMyVoicePage::slot_myVoiceListReplay);
	connect(ui.widget, &WPageCtlWidget::sig_changePage, this, &WMyVoicePage::slot_pageChange);
}

WMyVoicePage::~WMyVoicePage()
{}

void WMyVoicePage::initMyVoice()
{
	SettingInterfaceBussiness::getInstance()->getVoiceListReq(_cur_page, _page_size);
}

void WMyVoicePage::slot_pageChange(int index)
{
	SettingInterfaceBussiness::getInstance()->getVoiceListReq(index, _page_size);
}

void WMyVoicePage::on_pb_add_clicked()
{
	QDesktopServices::openUrl(QUrl("https://aisounda.cn/#/custom"));
}

void WMyVoicePage::slot_commonReplay(int type, bool success, const QString& msg)
{
	if (type == httpReqType::VoiceList_Req) {
		AiSound::GetInstance().ShowTip(this,msg);
	}
	else if (type == httpReqType::DelVoice_Req) {
		if (success) {
			initMyVoice();
		}
		else {
			AiSound::GetInstance().ShowTip(this, msg);
		}
	}
	else if (type == httpReqType::GetVoiceUrl_Req) {
		if (success) {
			if (!msg.isEmpty()) {
				AiSound::GetInstance().playVoiceMp3(msg);
			}
		}
		else {
			AiSound::GetInstance().ShowTip(this, msg);
		}
	}
}

void WMyVoicePage::slot_myVoiceListReplay(bool success, int, const strc_PageInfo page_info, const QVector<strc_MyVoice>& voice_list)
{
	if (success) {
		_total_page = page_info.total_pages;
		_cur_page = page_info.cur_page;
		_total_size = page_info.total_size;

		int index = 0;
		for (auto it : voice_list) {
			if (index >= _page_size) {
				break;
			}
			_voice_widget_list.at(index)->setVoice(it);
			_voice_widget_list.at(index)->setVisible(true);
			index++;
		}

		for (int i = index; i < _page_size; i++) {
			_voice_widget_list.at(i)->setVisible(false);
		}
		ui.label_page->setText(QString("%1/%2").arg(_cur_page).arg(_total_page));
		ui.widget->initCtl(_total_page, _total_size, _cur_page);
	}
}
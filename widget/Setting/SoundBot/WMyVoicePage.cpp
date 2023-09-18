#include "WMyVoicePage.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/AiSound.h"
#include <QDesktopServices>
#include "WMyVoiceModel.h"
#include "function/QAudioPlayer.h"


WMyVoicePage::WMyVoicePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (i == 0 && j == 0) {
				_add_widget = new WAddVoiceModel(this);
				ui.gridLayout->addWidget(_add_widget,i,j);
			}
			else {
				WMyVoiceModel* voice = new WMyVoiceModel(this);
				QSizePolicy sp_retain = voice->sizePolicy();
				sp_retain.setRetainSizeWhenHidden(true);
				voice->setSizePolicy(sp_retain);
				connect(voice, &WMyVoiceModel::sig_editMyVoice, this, &WMyVoicePage::slot_editMyVoice);
				connect(voice, &WMyVoiceModel::sig_playVoice, this, &WMyVoicePage::slot_playVoice);
				ui.gridLayout->addWidget(voice, i, j);
				_voice_widget_list.push_back(voice);
			}
		}
	}

	qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
	qRegisterMetaType<QVector<strc_MyVoice>>("QVector<strc_MyVoice>");
	connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_common_replay,this, &WMyVoicePage::slot_commonReplay);
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_myVoiceListReplay, this, &WMyVoicePage::slot_myVoiceListReplay);
	connect(ui.widget, &WPageCtlWidget::sig_changePage, this, &WMyVoicePage::slot_pageChange);

	_voice_editDlg = new WEditVoiceDlg(nullptr);
	_voice_editDlg->hide();

	connect(QAudioPlayer::GetInstance(), &QAudioPlayer::sig_playStop, this, [=] {
		if (_pre_model) {
			_pre_model->opeMoive(false);
			_pre_model = nullptr;
		}
		});
}

WMyVoicePage::~WMyVoicePage()
{
	if (_voice_editDlg) {
		delete _voice_editDlg;
		_voice_editDlg = nullptr;
	}
}

void WMyVoicePage::initMyVoice()
{
	SettingInterfaceBussiness::getInstance()->getVoiceListReq(_cur_page, _page_size);
}

void WMyVoicePage::slot_pageChange(int index)
{
	SettingInterfaceBussiness::getInstance()->getVoiceListReq(index, _page_size);
}

void WMyVoicePage::slot_editMyVoice(int voiceId, const QString& name, const QString& desc)
{
	_voice_editDlg->Show(voiceId, name,desc);
}

void WMyVoicePage::slot_commonReplay(int type, bool success, const QString& msg)
{
	if (type == httpReqType::VoiceList_Req) {
		AiSound::GetInstance().ShowTip(this,msg);
	}
	else if (type == httpReqType::DelVoice_Req || type == httpReqType::EditVoice) {
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
				//AiSound::GetInstance().playVoiceMp3(msg);
				QAudioPlayer::GetInstance()->playUrl(msg);
				if (_pre_model) {
					_pre_model->opeMoive(true);
				}
			}
			else {
				_pre_model = nullptr;
				AiSound::GetInstance().ShowTip(this, tr("Address is empty"));
			}
		}
		else {
			_pre_model = nullptr;
			AiSound::GetInstance().ShowTip(this, msg);
		}
	}
}

void WMyVoicePage::slot_playVoice(int voiceId)
{
	WMyVoiceModel* model = static_cast<WMyVoiceModel*>(QObject::sender());
	if (!model) {
		return;
	}
	if (_pre_model) {
		_pre_model->opeMoive(false);
	}
	_pre_model = model;
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
		_add_widget->setModelText(QString("%1/%2").arg(_cur_page).arg(_total_page));
		ui.widget->initCtl(_total_page, _total_size, _cur_page);
	}
}
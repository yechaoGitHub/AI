#include "WVoiceLibPage.h"
#include "WVoicelibWidget.h"
#include "function/AiSound.h"

WVoiceLibPage::WVoiceLibPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			WVoicelibWidget* widget = new WVoicelibWidget(this);
			ui.gridLayout->addWidget(widget, i, j);
		}
		ui.gridLayout->setRowStretch(i, 400);
	}

	QStringList gender_list{tr("gender"),tr("man"),tr("woman")};
	ui.comboBox_gender->addItems(gender_list);
	QStringList source_list{ tr("source"),tr("SoundaAI"),tr("User") };
	ui.comboBox_source->addItems(source_list);

	connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_common_replay,this,&WVoiceLibPage::slot_common_replay);
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_soundFilterReplay, this, &WVoiceLibPage::slot_soundFilterReplay);
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_soundLibReplay, this, &WVoiceLibPage::slot_soundLibReplay);
}

WVoiceLibPage::~WVoiceLibPage()
{}

void WVoiceLibPage::setSel()
{
	SettingInterfaceBussiness::getInstance()->getFilterListReq();
	strc_SoundType sound_type;
	SettingInterfaceBussiness::getInstance()->getSoundLIbReq(_cur_page, _page_size, sound_type);
}

void WVoiceLibPage::slot_common_replay(httpReqType type, bool success, const QString& msg)
{
	if (type == httpReqType::Filter_req || type == httpReqType::SoundLib_Req) {
		AiSound::GetInstance().ShowTip(this,msg);
	}
}

void WVoiceLibPage::slot_soundFilterReplay(bool success, int, const QString& msg, const  QVector<strc_SoundFilter>& filter_list)
{
	if (success) {
		_label_map.clear();
		_lang_map.clear();
		QStringList  label_list;
		label_list.push_back(tr("label"));
		QStringList  lang_list;
		lang_list.push_back(tr("language"));
		for (auto it : filter_list) {
			if (it.is_label) {
				label_list.push_back(it.value);
				_label_map[it.value] = it.id;
			}
			else {
				lang_list.push_back(it.value);
				_lang_map[it.value] = it.id;
			}
		}
		ui.comboBox_label->clear();
		ui.comboBox_lan->clear();
		ui.comboBox_label->addItems(label_list);
		ui.comboBox_lan->addItems(lang_list);
	}
}

void WVoiceLibPage::slot_soundLibReplay(bool, int, const strc_PageInfo page_info, const QVector<strc_SoundLib>& filter_list)
{

}
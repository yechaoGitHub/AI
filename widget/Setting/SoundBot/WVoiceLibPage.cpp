#include "WVoiceLibPage.h"
#include "WVoicelibWidget.h"
#include "function/AiSound.h"

WVoiceLibPage::WVoiceLibPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			WVoicelibWidget* widget = new WVoicelibWidget(this);
			ui.gridLayout->addWidget(widget, i, j);
			_voice_widget_list.push_back(widget);
		}
		ui.gridLayout->setRowStretch(i, 600);
	}
	ui.gridLayout->setSpacing(12);

	QStringList gender_list{tr("gender"),tr("man"),tr("woman")};
	ui.comboBox_gender->addItems(gender_list);
	QStringList source_list{ tr("source"),tr("SoundaAI"),tr("User") };
	ui.comboBox_source->addItems(source_list);

	qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
	qRegisterMetaType<QVector<strc_SoundLib>>("QVector<strc_SoundLib>");
	connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_common_replay,this,&WVoiceLibPage::slot_common_replay);

	//qRegisterMetaType<QVector<strc_SoundLib>>("QVector<strc_SoundFilter>");
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_soundFilterReplay, this, &WVoiceLibPage::slot_soundFilterReplay);
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_soundLibReplay, this, &WVoiceLibPage::slot_soundLibReplay);
}

WVoiceLibPage::~WVoiceLibPage()
{}

void WVoiceLibPage::setSel()
{
	static bool first = true;
	if (first) {
		first = false;
		SettingInterfaceBussiness::getInstance()->getFilterListReq();
	}
	else {
		SettingInterfaceBussiness::getInstance()->getSoundLIbReq(_cur_page, _page_size, _sound_type);
	}
}

void WVoiceLibPage::slot_comboxIndexChange(int index)
{
	if (QObject::sender() == ui.comboBox_gender) {
		_sound_type.gender = index;
	}
	else {
		_sound_type.source = index;
	}

	SettingInterfaceBussiness::getInstance()->getSoundLIbReq(1, _page_size, _sound_type);
}

void WVoiceLibPage::slot_comboxTextChange(const QString& text)
{
	int id = 0;
	if (QObject::sender() == ui.comboBox_label) {
		id = _label_map[text];
		_sound_type.label = id;
	}
	else {
		id = _lang_map[text];
		_sound_type.language = id;
	}

	SettingInterfaceBussiness::getInstance()->getSoundLIbReq(1, _page_size, _sound_type);
}

void WVoiceLibPage::slot_common_replay(int type, bool success, const QString& msg)
{
	if (type == httpReqType::Filter_req || type == httpReqType::SoundLib_Req) {
		AiSound::GetInstance().ShowTip(this,msg);
	}
	else if (type == httpReqType::AddVoice) {
		AiSound::GetInstance().ShowTip(this, msg);
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

void WVoiceLibPage::bindCombox(bool bind)
{
	if (bind) {
		connect(ui.comboBox_gender, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxIndexChange(int)));
		connect(ui.comboBox_source, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxIndexChange(int)));
		connect(ui.comboBox_lan, &QComboBox::currentTextChanged, this, &WVoiceLibPage::slot_comboxTextChange);
		connect(ui.comboBox_label, &QComboBox::currentTextChanged, this, &WVoiceLibPage::slot_comboxTextChange);
	}
	else {
		disconnect(ui.comboBox_gender, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxIndexChange(int)));
		disconnect(ui.comboBox_source, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxIndexChange(int)));
		disconnect(ui.comboBox_lan, &QComboBox::currentTextChanged, this, &WVoiceLibPage::slot_comboxTextChange);
		disconnect(ui.comboBox_label, &QComboBox::currentTextChanged, this, &WVoiceLibPage::slot_comboxTextChange);
	}
}

void WVoiceLibPage::slot_soundFilterReplay(bool success, int, const QString& msg, const  QVector<strc_SoundFilter>& filter_list)
{
	if (success) {
		bindCombox(false);
		_label_map.clear();
		_lang_map.clear();
		QStringList  label_list;
		label_list.push_back(tr("label"));
		_label_map["label"] = 0;
		QStringList  lang_list;
		lang_list.push_back(tr("language"));
		_lang_map["language"] = 0;
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
		bindCombox(true);

		SettingInterfaceBussiness::getInstance()->getSoundLIbReq(_cur_page, _page_size, _sound_type);
	}
}

void WVoiceLibPage::slot_soundLibReplay(bool success, int, const strc_PageInfo page_info, const QVector<strc_SoundLib>& filter_list)
{
	if (success) {
		ui.widget->initCtl(page_info.total_pages, page_info.total_size, page_info.cur_page);

		int index = 0;
		for (auto it : filter_list) {
			if (index >= _page_size) {
				break;
			}
			_voice_widget_list.at(index)->updateVoiceLib(it);
			_voice_widget_list.at(index)->show();
			index++;
		}
		for (int i = index; i < _page_size; i++) {
			_voice_widget_list.at(i)->hide();
		}
	}
}
#include "WMyVoiceModel.h"
#include <qstyle.h>
#include "function/Bussiness/SettingInterfaceBussiness.h"


WMyVoiceModel::WMyVoiceModel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_1->setProperty("man",true);
	ui.lb_1->style()->unpolish(ui.lb_1);

	ui.lb_content->setWordWrap(true);
}

WMyVoiceModel::~WMyVoiceModel()
{}

void WMyVoiceModel::setVoice(const strc_MyVoice& voice)
{
	ui.label_name->setText(voice.voiceName);
	ui.lb_content->setText(voice.description);

	_voiceType = voice.voiceType;
	_voiceId = voice.voiceId;

	if (voice.gender == 1) {
		ui.lb_1->setProperty("man", true);
	}
	else {
		ui.lb_1->setProperty("man", false);
	}

	ui.lb_1->style()->unpolish(ui.lb_1);
}

void WMyVoiceModel::on_del_btn_clicked()
{
	SettingInterfaceBussiness::getInstance()->delVoiceReq(_voiceId);
}

void WMyVoiceModel::on_pb_edit_clicked()
{

}
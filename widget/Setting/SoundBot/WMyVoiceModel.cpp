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

	_effect = new QLabel(this);
	_effect->setStyleSheet("border:none;background-color:rgba(0,0,0,0);");
	_effect->setFixedSize(QSize{ 15, 16 });

	//_movie = new QMovie(":/QtTest/icon/Setting/soundbot/voice_tick.png", "apng", this);
	//_movie = new QMovie(":/QtTest/icon/Setting/voice_gif.png", "apng", this);
	_movie = new QMovie(":/QtTest/icon/voice_tick.apng", "apng", this);

	_effect->setMovie(_movie);
	_effect->move(50, 122);
}

WMyVoiceModel::~WMyVoiceModel()
{}

void WMyVoiceModel::setVoice(const strc_MyVoice& voice)
{
	ui.label_name->setText(voice.voiceName);
	ui.lb_content->setText(voice.description);

	_voiceType = voice.voiceType;
	_voiceId = voice.voiceId;

	ui.lb_1->setProperty("man", voice.gender == 1);
	ui.lb_1->style()->unpolish(ui.lb_1);

	_movie->start();
	_movie->stop();
}

void WMyVoiceModel::on_del_btn_clicked()
{
	auto pos = ui.pb_use->pos();
	SettingInterfaceBussiness::getInstance()->delVoiceReq(_voiceId);
}

void WMyVoiceModel::on_pb_edit_clicked()
{
	emit sig_editMyVoice(_voiceId, ui.label_name->text(), ui.lb_content->text());
}

void WMyVoiceModel::opeMoive(bool start)
{
	if (start) {
		_movie->start();
	}
	else {
		_movie->stop();
	}
}

void WMyVoiceModel::on_pb_use_clicked()
{
	emit sig_playVoice(_voiceId);
	SettingInterfaceBussiness::getInstance()->getVoiceUrlReq(_voiceId);
}

void WMyVoiceModel::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}
#include "WVoicelibWidget.h"
#include <qstyle.h>
#include <QMovie>
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/AiSound.h"


WVoicelibWidget::WVoicelibWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_content->setWordWrap(true);
	ui.lb_1->setProperty("man",true);
	ui.lb_1->style()->unpolish(ui.lb_1);

	_effect = new QLabel(this);
	_effect->setStyleSheet("border:none;background-color:rgba(0,0,0,0);");
	_effect->setFixedSize(QSize{ 15, 16 });
	_movie = new QMovie(":/QtTest/icon/voice_tick.apng", "apng", this);

	_effect->setMovie(_movie);
	_effect->move(42, 122);
}

WVoicelibWidget::~WVoicelibWidget()
{}

void WVoicelibWidget::on_pb_add_clicked()
{
	emit sig_addVoice(_libId, ui.lb_name->text());
}

void WVoicelibWidget::on_pb_sample_clicked()
{
	emit sig_addVoice(_libId,"-1");
	SettingInterfaceBussiness::getInstance()->getVoiceUrlReq(_libId,false);
}

void WVoicelibWidget::opeMovie(bool start)
{
	if (start) {
		_movie->start();
	}
	else {
		_movie->stop();
	}
}

void WVoicelibWidget::updateVoiceLib(const strc_SoundLib& lib)
{
	_libId = lib.voiceLibId;
	if (lib.gender == 1) {
		ui.lb_1->setProperty("man", true);
	}
	else {
		ui.lb_1->setProperty("man", false);
	}
	ui.lb_1->style()->unpolish(ui.lb_1);

	ui.lb_name->setText(lib.voiceName);
	ui.lb_content->setText(lib.description);

	ui.lb_num->setText(QString::number(lib.clonedByCount));
	_movie->start();
	_movie->stop();
}
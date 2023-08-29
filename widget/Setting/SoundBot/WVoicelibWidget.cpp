#include "WVoicelibWidget.h"
#include <qstyle.h>
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/AiSound.h"


WVoicelibWidget::WVoicelibWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_content->setWordWrap(true);
	ui.lb_1->setProperty("man",true);
	ui.lb_1->style()->unpolish(ui.lb_1);
}

WVoicelibWidget::~WVoicelibWidget()
{}

void WVoicelibWidget::on_pb_add_clicked()
{
	SettingInterfaceBussiness::getInstance()->addMyVoice(_libId);
}

void WVoicelibWidget::on_pb_sample_clicked()
{
	SettingInterfaceBussiness::getInstance()->getVoiceUrlReq(_libId,false);
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
}
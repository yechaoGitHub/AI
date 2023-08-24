#include "WVoicelibWidget.h"
#include <qstyle.h>


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

void WVoicelibWidget::updateVoiceLib(const strc_SoundLib& lib)
{
	if (lib.gender == 1) {
		ui.lb_1->setProperty("man", true);
	}
	else {
		ui.lb_1->setProperty("man", false);
	}
	ui.lb_1->style()->unpolish(ui.lb_1);

	ui.lb_name->setText(lib.voiceName);
	ui.lb_content->setText(lib.description);
}
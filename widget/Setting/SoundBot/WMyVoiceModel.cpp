#include "WMyVoiceModel.h"
#include <qstyle.h>


WMyVoiceModel::WMyVoiceModel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_1->setProperty("man",true);
	ui.lb_1->style()->unpolish(ui.lb_1);
}

WMyVoiceModel::~WMyVoiceModel()
{}

#include "WLoginMainUi.h"
#include <qstyle.h>


WLoginMainUi::WLoginMainUi(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setUserSelect(true);
}

WLoginMainUi::~WLoginMainUi()
{}

void WLoginMainUi::on_pb_user_clicked()
{
	setUserSelect(true);
	ui.stackedWidget->setCurrentIndex(0);
}

void WLoginMainUi::on_pb_phone_clicked()
{
	setUserSelect(false);
	ui.stackedWidget->setCurrentIndex(1);
}

void WLoginMainUi::setUserSelect(bool sel)
{
	ui.pb_user->setProperty("is_sel", sel);
	ui.pb_user->style()->unpolish(ui.pb_user);
	ui.pb_user->style()->polish(ui.pb_user);

	ui.pb_phone->setProperty("is_sel", !sel);
	ui.pb_phone->style()->unpolish(ui.pb_user);
	ui.pb_phone->style()->polish(ui.pb_user);
}
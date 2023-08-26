#include "WSpeechSysPage.h"

WSpeechSysPage::WSpeechSysPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.pb_bk_2->setColor(QColor("#C9C9C9"));
}

WSpeechSysPage::~WSpeechSysPage()
{}


void WSpeechSysPage::slot_color_change()
{

}

void WSpeechSysPage::slot_bk_change()
{
	if (QObject::sender() == ui.pb_bk_1) {
		ui.pb_bk_1->setSel(true);
		ui.pb_bk_2->setSel(false);
	}
	else {
		ui.pb_bk_1->setSel(false);
		ui.pb_bk_2->setSel(true);
	}
}
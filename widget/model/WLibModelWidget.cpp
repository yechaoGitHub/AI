#include "WLibModelWidget.h"
#include <qstyle.h>


WLibModelWidget::WLibModelWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_content->setWordWrap(true);
	setSel(true);
	//this->installEventFilter(this);
}

WLibModelWidget::~WLibModelWidget()
{}

void WLibModelWidget::setSel(bool sel)
{
	ui.frame->setProperty("sel", sel);
	ui.frame->style()->unpolish(ui.frame);
	ui.lb_title->setProperty("sel", sel);
	ui.lb_title->style()->unpolish(ui.lb_title);
	ui.lb_content->setProperty("sel", sel);
	ui.lb_content->style()->unpolish(ui.lb_content);
}

void WLibModelWidget::mouseReleaseEvent(QMouseEvent* event)
{
	emit sig_libModel_click();
}

void WLibModelWidget::setTitle(const QString& title, const QString& content)
{
	ui.lb_title->setText(title);
	ui.lb_content->setText(content);
}
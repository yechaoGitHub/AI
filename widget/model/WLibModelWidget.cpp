#include "WLibModelWidget.h"
#include <qstyle.h>


WLibModelWidget::WLibModelWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lb_content->setWordWrap(true);
	setSel(false);
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
	_select = sel;
}

void WLibModelWidget::mouseReleaseEvent(QMouseEvent* event)
{
	//if(!_select)
	emit sig_libModel_click();
}

void WLibModelWidget::setTitle(const QString& title, const QString& content,int id)
{
	_model_id = id;
	ui.lb_title->setText(title);
	ui.lb_content->setText(content);
}

void WLibModelWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}
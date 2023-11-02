#include "WPageCtlWidget.h"
#include <QTimer>


WPageCtlWidget::WPageCtlWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pb_pre->setProperty("sel", false);
	ui.pb_pre->style()->unpolish(ui.pb_pre);
	ui.pb_next->setProperty("sel", false);
	ui.pb_next->style()->unpolish(ui.pb_next);
}

WPageCtlWidget::~WPageCtlWidget()
{}

void WPageCtlWidget::initCtl(int total_page, int total_size, int cur_page)
{
	_cur_page = cur_page;
	_total_pages = total_page;
	_total_size = total_size;
	ui.lb_total_size->setText(tr("entries of %1").arg(total_size));
	QStringList list;
	for (int i = 1; i <= total_page; i++) {
		list.append(QString::number(i));
	}

	disconnect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboxSelect(int)));
	ui.comboBox->clear();
	ui.comboBox->addItems(list);
	ui.comboBox->setCurrentIndex(cur_page-1);
	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboxSelect(int)));

	if (total_page == 0) {
		ui.frame_3->hide();
		ui.pb_next->hide();
		ui.pb_pre->hide();
	}
	else if (total_page == 1) {
		ui.pb_next->show();
		ui.pb_pre->show();
		ui.frame_3->show();
		ui.pb_2->hide();
		ui.pb_last->hide();
		ui.pb_about->hide();
		ui.pb_1->show();
		ui.pb_1->setText("1");
	}
	else if (total_page == 2) {
		ui.pb_next->show();
		ui.pb_pre->show();
		ui.frame_3->show();
		ui.pb_about->hide();
		ui.pb_last->hide();
		ui.pb_1->show();
		ui.pb_2->show();
		ui.pb_1->setText("1");
		ui.pb_2->setText("2");
	}
	else if (total_page == 3) {
		ui.pb_next->show();
		ui.pb_pre->show();
		ui.pb_1->show();
		ui.pb_2->show();
		ui.pb_last->show();
		ui.frame_3->show();
		ui.pb_about->hide();
		ui.pb_1->setText("1");
		ui.pb_2->setText("2");
		ui.pb_last->setText("3");
	}
	else {
		ui.pb_1->show();
		ui.pb_2->show();
		ui.pb_last->show();
		ui.pb_about->show();
		ui.pb_next->show();
		ui.pb_pre->show();
		ui.frame_3->show();

		ui.pb_last->setText(QString::number(_total_pages));
		if (cur_page + 2 < _total_pages) {
			ui.pb_1->setText(QString::number(cur_page));
			ui.pb_2->setText(QString::number(cur_page+1));
		}
		else {
			ui.pb_1->setText(QString::number(_total_pages-2));
			ui.pb_2->setText(QString::number(_total_pages-1));
		}
	}
	QTimer::singleShot(100, this, [=] {
		selCurIndex(_cur_page);
		});

}

void WPageCtlWidget::on_pb_next_clicked()
{
	if (_cur_page >= _total_pages) {
		return;
	}

	emit sig_changePage(_cur_page+1);
}

void WPageCtlWidget::on_pb_pre_clicked()
{
	if (_cur_page < 2) {
		return;
	}
	emit sig_changePage(_cur_page-1);
}

void WPageCtlWidget::onComboxSelect(int index)
{
	if (_cur_page != (index+1)) {
		emit sig_changePage(index+1);
	}
}

void WPageCtlWidget::on_pb_1_clicked()
{
	emit sig_changePage(ui.pb_1->text().toInt());
}

void WPageCtlWidget::on_pb_2_clicked()
{
	emit sig_changePage(ui.pb_2->text().toInt());
}

void WPageCtlWidget::on_pb_last_clicked()
{
	emit sig_changePage(ui.pb_last->text().toInt());
}

void WPageCtlWidget::selCurIndex(int index)
{
	int num_1=0, num_2=0, num_3 = 0;
	if (!ui.pb_1->isHidden()) {
		num_1 = ui.pb_1->text().toInt();
		ui.pb_1->setProperty("sel", num_1 == index);
		ui.pb_1->style()->unpolish(ui.pb_1);
	}

	if (!ui.pb_2->isHidden()) {
		num_2 = ui.pb_2->text().toInt();
		ui.pb_2->setProperty("sel", num_2 == index);
		ui.pb_2->style()->unpolish(ui.pb_2);
	}
	if (!ui.pb_last->isHidden()) {
		num_3 = ui.pb_last->text().toInt();
		ui.pb_last->setProperty("sel", num_3 == index);
		ui.pb_last->style()->unpolish(ui.pb_last);
	}


	if (num_3 > num_2 + 1) {
		ui.pb_about->setVisible(true);
	}
	else {
		ui.pb_about->setVisible(false);
	}
}

void WPageCtlWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);
		ui.lb_total_size->setText(tr("entries of %1").arg(_total_size));
	}

	QWidget::changeEvent(event);
}
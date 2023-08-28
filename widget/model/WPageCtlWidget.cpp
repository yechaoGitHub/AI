#include "WPageCtlWidget.h"

WPageCtlWidget::WPageCtlWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

WPageCtlWidget::~WPageCtlWidget()
{}

void WPageCtlWidget::initCtl(int total_page, int total_size, int cur_page)
{
	_total_pages = total_page;
	_cur_page = cur_page;

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

	if (total_page < 3) {
		ui.pb_next->hide();
		ui.pb_pre->hide();
		ui.frame_3->hide();
	}
	else if (total_page < 4) {
		ui.pb_next->show();
		ui.pb_pre->show();
		ui.frame_3->show();
		ui.pb_about->hide();
		ui.pb_1->setText("1");
		ui.pb_2->setText("2");
		ui.pb_last->setText("3");
	}
	else {
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
}

void WPageCtlWidget::on_pb_next_clicked()
{
	if (_cur_page >= _total_pages) {
		return;
	}

	emit sig_changePage(_cur_page++);
}

void WPageCtlWidget::on_pb_pre_clicked()
{
	if (_cur_page < 2) {
		return;
	}
	emit sig_changePage(_cur_page--);
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
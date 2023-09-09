#pragma once

#include <QWidget>
#include "ui_WPageCtlWidget.h"

class WPageCtlWidget : public QWidget
{
	Q_OBJECT
signals:
	void sig_changePage(int index);
public:
	WPageCtlWidget(QWidget *parent = nullptr);
	~WPageCtlWidget();

	void initCtl(int total_page,int total_size,int cur_page);

private slots:
	void on_pb_next_clicked();
	void on_pb_pre_clicked();
	void on_pb_1_clicked();
	void on_pb_2_clicked();
	void on_pb_last_clicked();

	void onComboxSelect(int);

private:
	void selCurIndex(int index);

private:
	Ui::WPageCtlWidgetClass ui;

	int	_cur_page = 0;
	int _total_pages = 0;
};

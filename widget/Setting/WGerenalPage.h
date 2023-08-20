#pragma once

#include <QWidget>
#include "ui_WGerenalPage.h"

class WGerenalPage : public QWidget
{
	Q_OBJECT

public:
	WGerenalPage(QWidget *parent = nullptr);
	~WGerenalPage();

private slots:
	void on_pb_send_clicked();

private:
	Ui::WGerenalPageClass ui;
};

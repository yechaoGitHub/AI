#pragma once

#include <QWidget>
#include "ui_WLoginMainUi.h"

class WLoginMainUi : public QWidget
{
	Q_OBJECT

public:
	WLoginMainUi(QWidget *parent = nullptr);
	~WLoginMainUi();

private slots:
	void on_pb_user_clicked();
	void on_pb_phone_clicked();

private:
	void setUserSelect(bool);

private:
	Ui::WLoginMainUiClass ui;
};

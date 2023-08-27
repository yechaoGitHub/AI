#pragma once

#include <QWidget>
#include "ui_WAccountPage.h"
#include "function/Bussiness/Define.h"


class WAccountPage : public QWidget
{
	Q_OBJECT

public:
	WAccountPage(QWidget *parent = nullptr);
	~WAccountPage();

	void initAccount(const stru_UserInfo& user_info);

private slots:
	void on_pb_charge_clicked();

private:
	Ui::WAccountPageClass ui;
};

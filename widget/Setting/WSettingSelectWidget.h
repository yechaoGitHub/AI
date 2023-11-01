#pragma once

#include <QWidget>
#include "ui_WSettingSelectWidget.h"
#include "function/Bussiness/Define.h"


class WSettingSelectWidget : public QWidget
{
	Q_OBJECT
signals:
	void sig_page_change(bool max);
	void sig_robot_clicked(bool click);
public:
	WSettingSelectWidget(QWidget *parent = nullptr);
	~WSettingSelectWidget();

	void initData();
	void initCheck();
private slots:
	void on_pb_account_clicked();
	void on_pb_help_clicked();
	void on_pb_team_clicked();
	void on_pb_tools_clicked();

	void slot_getUserInfoReplay(bool, int, const QString& msg, const stru_UserInfo& user_info);
private:
	void changeSelectBtn(WNavbarButton::BarType type);

protected:
	void changeEvent(QEvent* event) override;
private:
	Ui::WSettingSelectWidgetClass ui;
};

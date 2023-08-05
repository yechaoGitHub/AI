#pragma once

#include <QWidget>
#include "ui_WSettingSelectWidget.h"

class WSettingSelectWidget : public QWidget
{
	Q_OBJECT

public:
	WSettingSelectWidget(QWidget *parent = nullptr);
	~WSettingSelectWidget();

private slots:
	void on_pb_account_clicked();
	void on_pb_help_clicked();
	void on_pb_team_clicked();
	void on_pb_tools_clicked();
private:
	void changeSelectBtn(WNavbarButton::BarType type);

private:
	Ui::WSettingSelectWidgetClass ui;
};

#pragma once

#include <QWidget>
#include "ui_WHistoryDiaPage.h"

class WHistoryDiaPage : public QWidget
{
	Q_OBJECT

public:
	WHistoryDiaPage(QWidget *parent = nullptr);
	~WHistoryDiaPage();

private:
	Ui::WHistoryDiaPageClass ui;
};

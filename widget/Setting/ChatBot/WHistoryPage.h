#pragma once

#include <QWidget>
#include "ui_WHistoryPage.h"

class WHistoryPage : public QWidget
{
	Q_OBJECT

public:
	WHistoryPage(QWidget *parent = nullptr);
	~WHistoryPage();

private:
	Ui::WHistoryPageClass ui;
};

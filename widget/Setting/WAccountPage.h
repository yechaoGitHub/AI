#pragma once

#include <QWidget>
#include "ui_WAccountPage.h"

class WAccountPage : public QWidget
{
	Q_OBJECT

public:
	WAccountPage(QWidget *parent = nullptr);
	~WAccountPage();

private:
	Ui::WAccountPageClass ui;
};

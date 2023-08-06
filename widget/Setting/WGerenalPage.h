#pragma once

#include <QWidget>
#include "ui_WGerenalPage.h"

class WGerenalPage : public QWidget
{
	Q_OBJECT

public:
	WGerenalPage(QWidget *parent = nullptr);
	~WGerenalPage();

private:
	Ui::WGerenalPageClass ui;
};

#pragma once

#include <QWidget>
#include "ui_WLibarary.h"

class WLibarary : public QWidget
{
	Q_OBJECT

public:
	WLibarary(QWidget *parent = nullptr);
	~WLibarary();

private:
	Ui::WLibararyClass ui;
};

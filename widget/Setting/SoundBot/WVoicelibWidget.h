#pragma once

#include <QWidget>
#include "ui_WVoicelibWidget.h"

class WVoicelibWidget : public QWidget
{
	Q_OBJECT

public:
	WVoicelibWidget(QWidget *parent = nullptr);
	~WVoicelibWidget();

private:
	Ui::WVoicelibWidgetClass ui;
};

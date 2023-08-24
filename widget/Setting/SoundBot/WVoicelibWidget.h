#pragma once

#include <QWidget>
#include "ui_WVoicelibWidget.h"
#include "function/Bussiness/Define.h"


class WVoicelibWidget : public QWidget
{
	Q_OBJECT

public:
	WVoicelibWidget(QWidget *parent = nullptr);
	~WVoicelibWidget();

	void updateVoiceLib(const strc_SoundLib& lib);

private:
	Ui::WVoicelibWidgetClass ui;
};

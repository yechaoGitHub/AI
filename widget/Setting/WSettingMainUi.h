#pragma once

#include <QWidget>
#include "ui_WSettingMainUi.h"
#include "model/FrameLessWidget.h"


class WSettingMainUi : public FrameLessWidget
{
	Q_OBJECT

public:
	WSettingMainUi(QWidget *parent = nullptr);
	~WSettingMainUi();

	void Show(int type = 0);

protected:
	void paintEvent(QPaintEvent*) override;

private:
	Ui::WSettingMainUiClass ui;
};

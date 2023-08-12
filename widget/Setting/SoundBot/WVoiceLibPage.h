#pragma once

#include <QWidget>
#include "ui_WVoiceLibPage.h"

class WVoiceLibPage : public QWidget
{
	Q_OBJECT

public:
	WVoiceLibPage(QWidget *parent = nullptr);
	~WVoiceLibPage();

private:
	Ui::WVoiceLibPageClass ui;
};

#pragma once

#include <QWidget>
#include "ui_WMyVoicePage.h"

class WMyVoicePage : public QWidget
{
	Q_OBJECT

public:
	WMyVoicePage(QWidget *parent = nullptr);
	~WMyVoicePage();

private:
	Ui::WMyVoicePageClass ui;
};

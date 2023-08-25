#pragma once

#include <QWidget>
#include "ui_WMyVoicePage.h"


class WMyVoiceModel;
class WMyVoicePage : public QWidget
{
	Q_OBJECT

public:
	WMyVoicePage(QWidget *parent = nullptr);
	~WMyVoicePage();

private:
	Ui::WMyVoicePageClass ui;

	QVector<WMyVoiceModel*>		_voice_widget_list;
};

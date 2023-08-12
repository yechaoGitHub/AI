#pragma once

#include <QWidget>
#include "ui_WMyVoiceModel.h"

class WMyVoiceModel : public QWidget
{
	Q_OBJECT

public:
	WMyVoiceModel(QWidget *parent = nullptr);
	~WMyVoiceModel();

private:
	Ui::WMyVoiceModelClass ui;
};

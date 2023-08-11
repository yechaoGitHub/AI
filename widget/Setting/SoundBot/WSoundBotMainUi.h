#pragma once

#include <QWidget>
#include "ui_WSoundBotMainUi.h"

class WSoundBotMainUi : public QWidget
{
	Q_OBJECT

public:
	WSoundBotMainUi(QWidget *parent = nullptr);
	~WSoundBotMainUi();

private:
	Ui::WSoundBotMainUiClass ui;
};

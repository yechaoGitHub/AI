#pragma once

#include <QWidget>
#include "ui_WSpeechSysPage.h"

class WSpeechSysPage : public QWidget
{
	Q_OBJECT

public:
	WSpeechSysPage(QWidget *parent = nullptr);
	~WSpeechSysPage();

private:
	Ui::WSpeechSysPageClass ui;
};

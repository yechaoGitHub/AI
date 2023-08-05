#pragma once

#include <QWidget>
#include "ui_WChatDesc.h"

class WChatDesc : public QWidget
{
	Q_OBJECT
signals:
	void sig_startClick();
public:
	WChatDesc(QWidget *parent = nullptr);
	~WChatDesc();

private:
	Ui::WChatDescClass ui;
};

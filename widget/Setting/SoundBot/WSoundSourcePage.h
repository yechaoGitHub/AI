#pragma once

#include <QWidget>
#include "ui_WSoundSourcePage.h"

class WSoundSourcePage : public QWidget
{
	Q_OBJECT

public:
	WSoundSourcePage(QWidget *parent = nullptr);
	~WSoundSourcePage();

private:
	Ui::WSoundSourcePageClass ui;
};

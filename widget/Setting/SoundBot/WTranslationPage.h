#pragma once

#include <QWidget>
#include "ui_WTranslationPage.h"

class WTranslationPage : public QWidget
{
	Q_OBJECT

public:
	WTranslationPage(QWidget *parent = nullptr);
	~WTranslationPage();

private:
	Ui::WTranslationPageClass ui;
};

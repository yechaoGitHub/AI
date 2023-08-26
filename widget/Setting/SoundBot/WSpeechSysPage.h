#pragma once

#include <QWidget>
#include "ui_WSpeechSysPage.h"

class WSpeechSysPage : public QWidget
{
	Q_OBJECT

public:
	WSpeechSysPage(QWidget *parent = nullptr);
	~WSpeechSysPage();

private slots:
	void slot_color_change();
	void slot_bk_change();
private:
	Ui::WSpeechSysPageClass ui;

	QVector<WRadioBtn*>		_orig_btn_list;
};

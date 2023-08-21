#pragma once

#include <QWidget>
#include "ui_WHistoryDiaPage.h"


class soundHistoryModel;
class WHistoryDiaPage : public QWidget
{
	Q_OBJECT

public:
	WHistoryDiaPage(QWidget *parent = nullptr);
	~WHistoryDiaPage();

private slots:
	void slot_changePage(int index);

private:
	Ui::WHistoryDiaPageClass ui;
	soundHistoryModel* _sound_model = nullptr;


};

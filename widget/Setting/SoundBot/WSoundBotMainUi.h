#pragma once

#include <QWidget>
#include "ui_WSoundBotMainUi.h"

class WSoundBotMainUi : public QWidget
{
	Q_OBJECT
signals:
	void sig_soundBack();
public:
	WSoundBotMainUi(QWidget *parent = nullptr);
	~WSoundBotMainUi();

	void initCheck();
private:
	void changeSelectBtn(WNavbarButton::BarType type);

private slots:
	void on_pb_sound_page1_clicked();
	void on_pb_sound_page2_clicked();
	void on_pb_sound_page3_clicked();
	void on_pb_sound_page4_clicked();
	void on_pb_sound_page5_clicked();
	void on_pb_sound_page6_clicked();

	void slot_sound_back();
private:
	Ui::WSoundBotMainUiClass ui;
};

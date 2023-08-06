#pragma once

#include <QWidget>
#include "ui_WSpeechGenerationUi.h"
#include "model/FrameLessWidget.h"


class WSpeechGenerationUi : public FrameLessWidget
{
	Q_OBJECT

public:
	WSpeechGenerationUi(QWidget *parent = nullptr);
	~WSpeechGenerationUi();

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void on_pb_close_clicked();

private:
	Ui::WSpeechGenerationUiClass ui;
};

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

private:
	Ui::WSpeechGenerationUiClass ui;
};

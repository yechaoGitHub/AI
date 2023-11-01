#pragma once

#include <QWidget>
#include "ui_WSettingMainUi.h"
#include "model/FrameLessWidget.h"


class WSettingMainUi : public QWidget//FrameLessWidget
{
	Q_OBJECT

public:
	WSettingMainUi(QWidget *parent = nullptr);
	~WSettingMainUi();

	void Show(int type = 0);

protected:
	void paintEvent(QPaintEvent*) override;
	void resizeEvent(QResizeEvent* re) override;
	bool eventFilter(QObject* obj, QEvent* e);
	void changeEvent(QEvent* event) override;
private:
	Ui::WSettingMainUiClass ui;

	QPoint      pressedPoint_;
};

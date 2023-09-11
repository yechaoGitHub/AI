#pragma once

#include <QWidget>
#include "ui_WConformWidget.h"

class WConformWidget : public QWidget
{
	Q_OBJECT
signals:
	void   sig_conform();
public:
	WConformWidget(QWidget *parent = nullptr);
	~WConformWidget();

	void ShowConform(const QString& msg);
private slots:
	void on_pb_sure_clicked();
	void on_pb_cancel_clicked();

protected:
	void paintEvent(QPaintEvent*);

private:
	Ui::WConformWidgetClass ui;
};

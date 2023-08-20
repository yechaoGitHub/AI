#pragma once

#include <QWidget>
#include "ui_WLibModelWidget.h"

class WLibModelWidget : public QWidget
{
	Q_OBJECT
signals:
	void sig_libModel_click();
public:
	WLibModelWidget(QWidget *parent = nullptr);
	~WLibModelWidget();

	void setSel(bool sel);
	void setTitle(const QString& title,const QString& content);
protected:
	void mouseReleaseEvent(QMouseEvent* event);

private:
	Ui::WLibModelWidgetClass ui;
};

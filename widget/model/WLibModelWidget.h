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

	bool is_sel() {
		return _select;
	}

	int getModelId() {
		return _model_id;
	}

	void setSel(bool sel);
	void setTitle(const QString& title,const QString& content,int id);
protected:
	void mouseReleaseEvent(QMouseEvent* event);

private:
	Ui::WLibModelWidgetClass ui;
	bool  _select = false;
	int		_model_id = 0;
};

#pragma once

#include <QWidget>
#include "ui_WAddVoiceModel.h"

class WAddVoiceModel : public QWidget
{
	Q_OBJECT

public:
	WAddVoiceModel(QWidget *parent = nullptr);
	~WAddVoiceModel();

	void setModelText(const QString& text);

protected:
	void changeEvent(QEvent* event) override;
private slots:
	void	on_pb_add_clicked();
private:
	Ui::WAddVoiceModelClass ui;
};

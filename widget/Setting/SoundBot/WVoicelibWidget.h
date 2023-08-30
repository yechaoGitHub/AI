#pragma once

#include <QWidget>
#include "ui_WVoicelibWidget.h"
#include "function/Bussiness/Define.h"


class WVoicelibWidget : public QWidget
{
	Q_OBJECT
signals:
	void sig_addVoice(int voiceId,const QString& name);

public:
	WVoicelibWidget(QWidget *parent = nullptr);
	~WVoicelibWidget();

	void updateVoiceLib(const strc_SoundLib& lib);

private slots:
	void on_pb_add_clicked();
	void on_pb_sample_clicked();

private:
	Ui::WVoicelibWidgetClass ui;
	int		_libId = 0;
};

#pragma once

#include <QDialog>
#include "ui_WEditVoiceDlg.h"
#include "model/FrameLessWidget.h"


class WEditVoiceDlg : public FrameLessWidget
{
	Q_OBJECT

public:
	WEditVoiceDlg(QWidget *parent = nullptr);
	~WEditVoiceDlg();

	void Show(int voiceId, const QString& name, const QString& desc);

private slots:
	void on_btn_close_clicked();
	void on_pb_cancel_clicked();
	void on_pb_sure_clicked();

protected:
	void paintEvent(QPaintEvent*) override;

private:
	Ui::WEditVoiceDlgClass ui;

	int _voiceId = 0;
};

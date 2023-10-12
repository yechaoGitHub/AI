#pragma once

#include <QDialog>
#include "ui_WConformDlg.h"
#include "model/FrameLessWidget.h"


class WConformDlg : public QDialog
{
	Q_OBJECT

public:
	WConformDlg(QWidget *parent = nullptr);
	~WConformDlg();

	void Show(int voiceId,const QString& name);

private slots:
	void on_btn_close_clicked();
	void on_pb_cancel_clicked();
	void on_pb_sure_clicked();

protected:
	void paintEvent(QPaintEvent*) override;

private:
	Ui::WConformDlgClass ui;

	int _voiceId = 0;
};

#pragma once

#include <QDialog>
#include "ui_QAddVoiceDlg.h"
#include "model/FrameLessWidget.h"


class QAddVoiceDlg : public QDialog
{
	Q_OBJECT

public:
	QAddVoiceDlg(QWidget *parent = nullptr);
	~QAddVoiceDlg();

	void Show(int voiceId,const QString& name);

protected:
	void changeEvent(QEvent* event) override;

private slots:
	void on_btn_close_clicked();
	void on_pb_cancel_clicked();
	void on_pb_sure_clicked();
	void on_toolButton_clicked();

protected:
	void paintEvent(QPaintEvent*) override;

private:
	Ui::QAddVoiceDlgClass ui;

	int _voiceId = 0;
};

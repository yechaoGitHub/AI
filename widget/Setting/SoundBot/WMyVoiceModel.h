#pragma once

#include <QWidget>
#include "ui_WMyVoiceModel.h"
#include "Bussiness/Define.h"
#include <QMovie>


class WMyVoiceModel : public QWidget
{
	Q_OBJECT
signals:
	void sig_editMyVoice(int voiceId,const QString& name,const QString& desc);
public:
	WMyVoiceModel(QWidget *parent = nullptr);
	~WMyVoiceModel();

	void	setVoice(const strc_MyVoice& voice);

private slots:
	void on_del_btn_clicked();
	void on_pb_edit_clicked();
	void on_pb_use_clicked();
private:
	Ui::WMyVoiceModelClass ui;

	QLabel* _effect = nullptr;
	QMovie* _movie = nullptr;

	int		_voiceType = 0;  //��Դ(�̶�):1.��ɫ���ɣ�2.��ݺ�ʵ(���᷵��)��3.voiceLib���
	int		_voiceId = 0;
};

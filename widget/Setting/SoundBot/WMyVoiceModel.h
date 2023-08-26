#pragma once

#include <QWidget>
#include "ui_WMyVoiceModel.h"
#include "Bussiness/Define.h"

class WMyVoiceModel : public QWidget
{
	Q_OBJECT

public:
	WMyVoiceModel(QWidget *parent = nullptr);
	~WMyVoiceModel();

	void	setVoice(const strc_MyVoice& voice);

private slots:
	void on_del_btn_clicked();
	void on_pb_edit_clicked();
private:
	Ui::WMyVoiceModelClass ui;

	int		_voiceType = 0;  //��Դ(�̶�):1.��ɫ���ɣ�2.��ݺ�ʵ(���᷵��)��3.voiceLib���
	int		_voiceId = 0;
};

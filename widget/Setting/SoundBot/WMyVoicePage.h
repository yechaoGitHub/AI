#pragma once

#include <QWidget>
#include "ui_WMyVoicePage.h"
#include "function/Bussiness/Define.h"


class WMyVoiceModel;
class WMyVoicePage : public QWidget
{
	Q_OBJECT

public:
	WMyVoicePage(QWidget *parent = nullptr);
	~WMyVoicePage();

	void initMyVoice();

private slots:
	void	slot_commonReplay(int type, bool, const QString& msg);
	void	slot_myVoiceListReplay(bool, int, const strc_PageInfo page_info, const QVector<strc_MyVoice>& voice_list);
private:
	Ui::WMyVoicePageClass ui;

	QVector<WMyVoiceModel*>		_voice_widget_list;

	int		_cur_page = 1;
	int		_total_page = 0;
	int		_total_size = 0;
	int		_page_size = 5;
};

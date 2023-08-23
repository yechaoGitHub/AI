#pragma once

#include <QWidget>
#include "ui_WVoiceLibPage.h"
#include "Bussiness/SettingInterfaceBussiness.h"

class WVoiceLibPage : public QWidget
{
	Q_OBJECT

public:
	WVoiceLibPage(QWidget* parent = nullptr);
	~WVoiceLibPage();

	void	setSel();

private slots:
	void	slot_common_replay(httpReqType type, bool success, const QString& msg);
	void	slot_soundFilterReplay(bool, int, const QString& msg, const  QVector<strc_SoundFilter>& filter_list);
	void	slot_soundLibReplay(bool, int, const strc_PageInfo page_info, const QVector<strc_SoundLib>& filter_list);

private:
	Ui::WVoiceLibPageClass ui;

	QMap<QString, int>	_label_map;
	QMap<QString, int>	_lang_map;

	int		_cur_page = 1;
	int		_page_size = 4;
	int		_total_size = 0;
	int		_total_pages = 0;
};
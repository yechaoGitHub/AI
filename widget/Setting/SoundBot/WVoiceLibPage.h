#pragma once

#include <QWidget>
#include "ui_WVoiceLibPage.h"
#include "Bussiness/SettingInterfaceBussiness.h"
#include "QAddVoiceDlg.h"

class WVoicelibWidget;
class WVoiceLibPage : public QWidget
{
	Q_OBJECT

public:
	WVoiceLibPage(QWidget* parent = nullptr);
	~WVoiceLibPage();

	void	setSel();

private slots:
	void	slot_common_replay(int type, bool success, const QString& msg);
	void	slot_soundFilterReplay(bool, int, const QString& msg, const  QVector<strc_SoundFilter>& filter_list);
	void	slot_soundLibReplay(bool, int, const strc_PageInfo page_info, const QVector<strc_SoundLib>& filter_list);

	void	slot_comboxIndexChange(int index);
	void	slot_comboxTextChange(const QString& index);

	void	slot_addVoice(int voiceID,const QString& name);
private:
	void	bindCombox(bool bind);

private:
	Ui::WVoiceLibPageClass ui;
	QVector<WVoicelibWidget*>  _voice_widget_list;
	QAddVoiceDlg* _addVoiceDlg = nullptr;
	QMap<QString, int>	_label_map;
	QMap<QString, int>	_lang_map;
	strc_SoundType _sound_type;

	int		_cur_page = 1;
	int		_page_size = 6;
	int		_total_size = 0;
	int		_total_pages = 0;

	WVoicelibWidget* _cur_lib_wgt = nullptr;
};
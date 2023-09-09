#pragma once

#include <QWidget>
#include "ui_WHistoryDiaPage.h"
#include "Bussiness/Define.h"


class soundHistoryModel;
class WHistoryDelegate;
class WHistoryDiaPage : public QWidget
{
	Q_OBJECT

public:
	WHistoryDiaPage(QWidget *parent = nullptr);
	~WHistoryDiaPage();

	void  getTransHistory();
	void  opeHistoryItem(int type,int index);
//protected:
//	void keyReleaseEvent(QKeyEvent* event); //¼üÅÌËÉ¿ªÊÂ¼þ

private slots:
	void slot_changePage(int index);
	void on_pb_search_clicked();
	void slot_commonReplay(int ,bool,const QString& msg);
	void slot_transHistoryReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_transHistory>& chat_info);
private:
	Ui::WHistoryDiaPageClass ui;
	soundHistoryModel* _sound_model = nullptr;
	WHistoryDelegate* _history_delegate = nullptr;
	QVector<strc_transHistory>   _trans_info_list;

	int		_cur_page = 1;
	int		_total_size = 0;
	int		_total_pages = 1;
	int		_page_size = 10;
};

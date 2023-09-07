#pragma once

#include <QWidget>
#include "ui_WHistoryPage.h"
#include "Bussiness/Define.h"


class WHistoryDelegate;
class historyListModel;
class WHistoryPage : public QWidget
{
	Q_OBJECT

public:
	WHistoryPage(QWidget *parent = nullptr);
	~WHistoryPage();

	void  reqChatHistory();

private slots:
	void	on_pb_search_clicked();
	void	slot_changePage(int index);
	void	slot_chatHistoryReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_ChatHistory>& chat_info);
	void	slot_commonReplay(int type,bool,const QString& msg);
private:
	void	userOpe(int type,int index);

private:
	Ui::WHistoryPageClass ui;
	historyListModel* _history_model = nullptr;
	WHistoryDelegate* _history_delegate = nullptr;

	QVector<strc_ChatHistory>  chat_list_;
	int    _total_pages = 0;
	int	   _cur_page = 1;
	int    _total_size = 0;
	int    _page_size = 10;
	QString _search_text;
};

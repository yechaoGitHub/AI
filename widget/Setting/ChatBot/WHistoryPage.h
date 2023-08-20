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
	void on_pb_search_clicked();
	void slot_changePage(int index);
	void	slot_chatHistoryReplay(bool, int, const QString& msg, const  QVector<strc_ChatHistory>& chat_info);
private:
	Ui::WHistoryPageClass ui;
	historyListModel* _history_model = nullptr;
	WHistoryDelegate* _history_delegate = nullptr;

	int    _total_pages = 0;
	int	   _cur_page = 1;
	int    _total_size = 0;
};

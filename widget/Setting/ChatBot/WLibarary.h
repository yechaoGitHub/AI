#pragma once

#include <QWidget>
#include "ui_WLibarary.h"
#include "function/Bussiness/Define.h"
#include "model/WLibModelWidget.h"


class WLibarary : public QWidget
{
	Q_OBJECT

public:
	WLibarary(QWidget *parent = nullptr);
	~WLibarary();

	void	updateLib(const QVector<strc_ChatbotInfo>& chatbot_list);
	void	updateLibBySelType(int type);

private slots:
	void	slot_model_clicked();
	void    slot_page_change(int index);
private:
	Ui::WLibararyClass ui;

	QVector<strc_ChatbotInfo>		_chatBot_list;
	QVector<strc_ChatbotInfo>		_chatBot__type_list;
	QVector<WLibModelWidget*>		_lib_model_list;
	int		_cur_type = -1;

	int		_pages = 1;
	int		_total_size = 0;
	int		_cur_page = 1;
	int		_page_size = 6;
};

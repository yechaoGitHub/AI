#pragma once

#include <QWidget>
#include "ui_WLibarary.h"
#include "function/Bussiness/Define.h"
#include "model/WLibModelWidget.h"
#include "model/WConformDlg.h"


class WLibarary : public QWidget
{
	Q_OBJECT
public:
	WLibarary(QWidget *parent = nullptr);
	~WLibarary();

	void	updateLibBySelType(int type);

	void	getChatBotTemplate();

protected:
	void changeEvent(QEvent* event) override;

private slots:
	void	slot_model_clicked();
	void    slot_page_change(int index);

	void	slot_getChatBotListReplay(bool, int, const strc_PageInfo& page_info, const QVector<strc_ChatbotInfo>& user_info);

private:
	void opeModelswitch(bool open,int id);
private:
	Ui::WLibararyClass ui;
	WConformDlg* _conform_dlg = nullptr;

	QVector<WLibModelWidget*>		_lib_model_list;
	int		_cur_type = -1;
	QStringList		_model_list;
	WLibModelWidget* _select_model = nullptr;
	int		_pages = 1;
	int		_total_size = 0;
	int		_cur_page = 1;
	int		_page_size = 6;
};

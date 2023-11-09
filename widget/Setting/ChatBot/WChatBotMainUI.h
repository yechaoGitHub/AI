#pragma once

#include <QWidget>
#include "ui_WChatBotMainUI.h"
#include "function/Bussiness/Define.h"
#include "model/WLabelButton.h"
#include <QMap>


class WChatBotMainUI : public QWidget
{
	Q_OBJECT
signals:
	void sig_chatBotBack();
public:
	WChatBotMainUI(QWidget *parent = nullptr);
	~WChatBotMainUI();

	void initCheck();
	void changeSelectBtn(WNavbarButton::BarType type);

protected:
	void changeEvent(QEvent* event) override;

private slots:
	void on_pb_lib_clicked();
	void on_pb_history_clicked();
	void slot_type_btn_clicked();
	void slot_back();

	void	slot_getChatBotType(bool,int,const QMap<int,QString>&);
private:
	Ui::WChatBotMainUIClass ui;
	WLabelButton* _pre_select_btn = nullptr;
	WLabelButton* _all_btn = nullptr;
	WNavbarButton::BarType			_cur_type;
	QVector<WLabelButton*>			_chatBot_btn_list;
};

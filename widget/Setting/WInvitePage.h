#pragma once

#include <QWidget>
#include "ui_WInvitePage.h"
#include "function/Bussiness/Define.h"

class teamListModel;
class WTeamDelegate;
class WInvitePage : public QWidget
{
	Q_OBJECT

public:
	WInvitePage(QWidget *parent = nullptr);
	~WInvitePage();

	void reqTeamData();

private slots:
	void on_pb_invite_clicked();
	void on_pb_search_clicked();

	void slot_changePage(int index);

	void slot_inviteReplay(int type, bool, const QString& msg);
	void slot_searchTeam_replay(int, int total,int, const QVector<struc_teamInfo>& user_info);

protected:
	void changeEvent(QEvent* event) override;

private:
	Ui::WInvitePageClass ui;
	teamListModel*	_team_model = nullptr;
	WTeamDelegate* _team_delegate = nullptr;
	QVector<struc_teamInfo>     _teal_list;

	int		_page_size = 8;
	int		_total_size = 0;
	int		_total_pages = 0;
	int		_cur_page = 1;
};

#pragma once

#include <QObject>
#include <QMutex>
#include "Define.h"


class SettingInterfaceBussiness  : public QObject
{
	Q_OBJECT
signals:
	void	sig_getUserInfoReplay(bool,int, const QString& msg,const stru_UserInfo& user_info);
	void	sig_searchTeam_replay(int pages, int cur_page,int total,const QVector<struc_teamInfo>& user_info);
	void	sig_getChatBotListReplay(bool, int, const QString& msg, const  QVector<strc_ChatbotInfo>& user_info);
	void	sig_chatHistoryReplay(bool, int, const QString& msg, const  QVector<strc_ChatHistory>& chat_info);

	void	sig_common_replay(httpReqType type,bool success,const QString& msg);
public:
	SettingInterfaceBussiness(QObject *parent);
	~SettingInterfaceBussiness();

	static SettingInterfaceBussiness* getInstance();
	static SettingInterfaceBussiness* _instance;

	// 获取用户信息请求
	void getUserInfoReq();

	// 邀请入团
	void inviteUserJoinReq(const QString& username);
	// 团队成员
	void getTeamRecordReq(int page,int pageSize,const QString& search);
	// 移除成员
	void removeTeamReq(qint64 userId);

	void feedBackReq(const QString& msg);

	// 获取机器人模板list
	void getCharBotListReq();

	void getCharHistoryReq(int type, int page, const QString& search, int pageSize=10);

private:
	void paraseHttpResponse(httpReqType req_type,const QString& response);
};

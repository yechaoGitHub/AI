#pragma once
#include <QString>


enum netCode {
	Success = 200,
	TimeOut = -1,
	ServerErr = -2,
};

enum httpReqType {
	UserInfo_Req = 0,
	InviteUser_Req,
	Search_Team,
	Remove_Team,
	Feedback,
	ChatBot_req,
	ChatHistory_Req,
};

struct stru_UserInfo {
	qint32 userId = 0;
	QString phoneId;
	QString userName;
	double	balance = 0.0f;
};

struct struc_teamInfo {
	QString username;
	double  credits_used;
	QString join_time;
	int status;
	qint64  userId = 0;
};

struct strc_ChatbotInfo {
	QString  typeName;
	int		 type = 0;
	QString  name;
	QString  desc;
	QString  initialMessage;
	bool     isRecommend = false;
};

struct strc_ChatHistory {
	int		chatHistoryId = 0;
	int		chatType = 1;
	QString content;
	QString initTime;
	qint32  receiverId = 0;
	qint32  senderId = 0;
};
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
	Filter_req,
	ChatHistory_Req,
	SoundLib_Req,
	AddVoice,
	VoiceList_Req,
	DelVoice_Req,
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

struct strc_SoundFilter {
	bool		is_label = false;
	QString		value;
	int			id = 0;
};

struct strc_SoundType {
	int		gender = 0;
	int		label = 0;
	int		language = 0;
	int     source = 0;
};

struct strc_SoundLib {
	QString		description;
	int			gender = 0;
	int			label = 0;
	int			language = 0;
	int			source = 0;
	int			voiceLibId = 0;
	int			clonedByCount = 0;
	QString		voiceName;
};

struct strc_MyVoice {
	QString description;
	int		voiceId = 0;
	QString voiceName;
	int		voiceType = 0;
	int		gender = 1;
};

struct strc_PageInfo {
	int		total_size = 0;
	int		page_size = 4;
	int		total_pages = 0;
	int		cur_page = 1;
};
#pragma once

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QMap>
#include "Define.h"


class SettingInterfaceBussiness  : public QObject
{
	Q_OBJECT
signals:
	void	sig_getUserInfoReplay(bool,int, const QString& msg,const stru_UserInfo& user_info);
	void	sig_searchTeam_replay(int pages, int cur_page,int total,const QVector<struc_teamInfo>& user_info);
	void	sig_getChatBotListReplay(bool, int, const strc_PageInfo&, const  QVector<strc_ChatbotInfo>& user_info);
	void	sig_chatHistoryReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_ChatHistory>& chat_info);
	void	sig_soundFilterReplay(bool, int, const QString& msg, const  QVector<strc_SoundFilter>& filter_list);
	void	sig_soundLibReplay(bool, int,const strc_PageInfo page_info,const QVector<strc_SoundLib>& filter_list);
	void	sig_myVoiceListReplay(bool, int, const strc_PageInfo page_info, const QVector<strc_MyVoice>& voice_list);
	void	sig_transHistoryReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_transHistory>& chat_info);
	void	sig_chatBotListReplay(bool, int,const QMap<int,QString>&);

	void	sig_chatRecordReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_chatRecord>& chat_list);

	void	sig_common_replay(int type,bool success,const QString& msg);

public:
	SettingInterfaceBussiness(QObject *parent = nullptr);
	~SettingInterfaceBussiness();

	static SettingInterfaceBussiness* getInstance();
	static SettingInterfaceBussiness* _instance;

	void	initThread();
	void	uninitialize();

	void	getGlobalInfo();
	Q_INVOKABLE void _getGlobalInfo();

	// 获取用户信息请求
	void getUserInfoReq();
	Q_INVOKABLE void _getUserInfoReq();

	// 邀请入团
	void inviteUserJoinReq(const QString& username);
	// 团队成员
	void getTeamRecordReq(int page,int pageSize,const QString& search);
	Q_INVOKABLE void _getTeamRecordReq(int page, int pageSize, const QString& search);
	// 移除成员
	void removeTeamReq(qint64 userId);
	Q_INVOKABLE void _removeTeamReq(qint64 userId);

	void feedBackReq(const QString& msg);

	// 获取机器人模板list
	void getChatBotType();
	void getCharBotListReq(int page,int page_size,int type=-1);
	Q_INVOKABLE void _getCharBotListReq(int page, int page_size,int type);

	void getChatHistoryReq(int page, const QString& search, int pageSize=10);
	void delChatHsitory(const QStringList& chatId_list);

	// 获取声音filter列表
	void getFilterListReq();
	void getSoundLIbReq(int pageNo, int page, strc_SoundType sound_type);
	Q_INVOKABLE void _getSoundLIbReq(int pageNo, int page, strc_SoundType sound_type);

	//voice
	void addMyVoice(int libId,const QString& voiceName);
	void getVoiceListReq(int cur_page,int page_size);
	Q_INVOKABLE void _getVoiceListReq(int cur_page, int page_size);
	void delVoiceReq(int voiceId);
	void editMyVoice(int libId, const QString& voiceName,const QString& desc);

	void getVoiceUrlReq(int voiceId,bool my_voice = true);
	Q_INVOKABLE void _getVoiceUrlReq(int voiceId);
	Q_INVOKABLE void _getVoiceLibUrlReq(int voiceId);

	void getTransHistory(int page_size,int pageNo,const QString& text);
	void delTransId(int id);

	void getChatRecord(int page_size, int pageNo, const QString& chatId);
	Q_INVOKABLE void _getChatRecord(int page_size, int pageNo, const QString& chatId);
private:
	void paraseHttpResponse(httpReqType req_type,const QString& response);

	QThread* _thread = nullptr;
	QMutex   _mutex;
};

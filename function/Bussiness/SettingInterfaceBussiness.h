#pragma once

#include <QObject>
#include <QMutex>
#include "Define.h"


class SettingInterfaceBussiness  : public QObject
{
	Q_OBJECT
signals:
	void	sig_getUserInfoReplay(bool,int, const QString& msg,const stru_UserInfo& user_info);
public:
	SettingInterfaceBussiness(QObject *parent);
	~SettingInterfaceBussiness();

	static SettingInterfaceBussiness* getInstance();
	static SettingInterfaceBussiness* _instance;

	// ��ȡ�û���Ϣ����
	void getUserInfoReq();

private:

};

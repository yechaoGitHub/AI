#pragma once
#include <QString>


enum netCode {
	Success = 200,
	TimeOut = -1,
	ServerErr = -2,
};


struct stru_UserInfo {
	qint32 userId = 0;
	QString phoneId;
	QString userName;
	qint64	balance = 0;
};

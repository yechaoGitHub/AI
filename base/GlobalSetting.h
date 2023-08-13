#ifndef _GLOBALSETTING
#define _GLOBALSETTING

#include <QObject>
#include <QMutex>

class QSettings;
class GlobalSetting
{
public:
    GlobalSetting();
    ~GlobalSetting();

    bool init(const QString& path);
    QString getRootLocalPath();

    QString getHostAddress();

    QString getUserName();
    void setUserName(const QString& userName);

    QString getPWD();
    void setPWD(const QString& pwd);

    bool getRememberPWD();
    void setRememberPWD(bool b);

    bool getAutoLogin();
    void setAutoLogin(bool b);

    void setToken(const QString& token);
    QString getToken();

private:
    QSettings       *m_pSettings = nullptr;
    QMutex           m_mutex;
    QString         m_strToken;
    QString         m_strCurPwd;

};

extern GlobalSetting SETTING;
#endif
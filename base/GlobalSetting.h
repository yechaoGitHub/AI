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

    QString getUserName();
    void setUserName(const QString& userName);

    QString getPWD();
    void setPWD(const QString& pwd);

    bool getRememberPWD();
    void setRememberPWD(bool b);

    bool getAutoLogin();
    void setAutoLogin(bool b);

private:
    QSettings       *m_pSettings = nullptr;
    QMutex           m_mutex;
    QString         m_strCurPwd;

};

extern GlobalSetting SETTING;
#endif
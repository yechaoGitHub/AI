#ifndef _GLOBALSETTING
#define _GLOBALSETTING

#include <QObject>
#include <QMutex>
#include <QAudioDeviceInfo>

class QSettings;
class GlobalSetting
{
public:
    GlobalSetting();
    ~GlobalSetting();

    void setRebotModel(int model);
    int getRebotModel();

    void setRechargeUrl(const QString& url);
    QString getRechargeUrl();

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

    void setCurLanguage(int language);
    int  getCurLanguage();

    void setRobotBot(bool open);
    bool getRobotBot();

    void setSoundBot(bool open);
    bool getSoundBot();

    void setSpeechBk(int bk);
    int  getSpeechBk();

    void setSpeechOg(int bk);
    int  getSpeechOg();

    QColor getSpeechBkColor();
    QColor getSpeechOgColor();

    void setTransBk(int bk);
    int  getTransBk();

    void setTransOg(int bk);
    int  getTransOg();

    void setTransTl(int bk);
    int  getTransTl();

    void setTransTT(int bk);
    int  getTransTT();

    void setTransSD(int bk);
    int  getTransSD();

    QColor getTransBkColor();
    QColor getTransOgColor();
    QColor getTransColor();

    QString getMicDeviceName();
    void setMicDeviceName(const QString& name);

    QString getSpeakerDeviceName();
    void setSpeakerDeviceName(const QString& name);

    QString getMonitorDeviceName();
    void setMonitorDeviceName(const QString& name);

private:
    QSettings           *m_pSettings = nullptr;
    QMutex              m_mutex;
    QString             m_strCurPwd;

    QString             _micDeviceName;
    QString             _speakerDeviceName;
    QString             _monitorDeviceName;

    QString             m_strRechargeUrl;
};

extern GlobalSetting SETTING;
#endif
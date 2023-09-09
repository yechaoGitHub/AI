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

    void setToken(const QString& token);
    QString getToken();

    void setRobotBot(bool open);
    bool getRobotBot();

    void setSoundBot(bool open);
    bool getSoundBot();

    void setSpeechBk(int bk);
    int  getSpeechBk();

    void setSpeechOg(int bk);
    int  getSpeechOg();

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

    QAudioDeviceInfo& MicDeviceInfo();
    QAudioDeviceInfo& SpeakerDeviceInfo();
    QAudioDeviceInfo& MonitorDeviceInfo();

private:
    QSettings           *m_pSettings = nullptr;
    QMutex              m_mutex;
    QString             m_strToken;
    QString             m_strCurPwd;
    QAudioDeviceInfo    m_micDeviceInfo;
    QAudioDeviceInfo    m_speakerDeviceInfo;
    QAudioDeviceInfo    m_monitorDeviceInfo;

    QString             m_strRechargeUrl;
};

extern GlobalSetting SETTING;
#endif
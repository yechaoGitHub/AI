#include "GlobalSetting.h"
#include "AiSound.h"

#include <QSettings>
#include <QStandardPaths>


GlobalSetting SETTING;

#define VERSION_NAME  QString("V 1.0.1")

GlobalSetting::GlobalSetting()
{

}

GlobalSetting::~GlobalSetting()
{
    if (m_pSettings) {
        delete m_pSettings;
        m_pSettings = nullptr;
   }
}

bool GlobalSetting::init(const QString& path)
{
    if (m_pSettings)
        return false;

    auto& ins = AiSound::GetInstance();
    auto&& list = ins.GetOutputDeviceList();
    if (list.size())
    {
        m_speakerDeviceInfo = list[0];
    }

    list = ins.GetInputDeviceList();
    if (list.size())
    {
        m_micDeviceInfo = list[0];
    }

    m_monitorDeviceInfo = m_micDeviceInfo;

    for (auto& in : list)
    {
        if (in.deviceName().indexOf("VoiceMeeter Output") != -1)
        {
            m_monitorDeviceInfo = in;
            break;
        }
    }

    m_pSettings = new QSettings(path, QSettings::IniFormat);
    if (m_pSettings)
        return true;
    return false;
}

void GlobalSetting::setRebotModel(int model)
{
    if (m_pSettings)
        m_pSettings->setValue("Robot/model", model);
}

int GlobalSetting::getRebotModel()
{
    if (!m_pSettings)
        return 0;
    return m_pSettings->value("Robot/model", 0).toInt();
}

void GlobalSetting::setRechargeUrl(const QString& url)
{
    m_strRechargeUrl = url;
}

QString GlobalSetting::getRechargeUrl()
{
    return m_strRechargeUrl;
}

QString GlobalSetting::getRootLocalPath()
{
    return QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first();
}

QString GlobalSetting::getHostAddress()
{
    if (!m_pSettings)
        return QString();
    return m_pSettings->value("Login/hostAddress", "http://47.106.253.9:9101").toString();
}

void GlobalSetting::setToken(const QString& token)
{
    m_strToken = token;
}

QString GlobalSetting::getToken()
{
    return m_strToken;
}

void GlobalSetting::setCurLanguage(int language)
{
    if (m_pSettings)
        m_pSettings->setValue("Setting/language", language);
}

int  GlobalSetting::getCurLanguage()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Setting/language", 1).toInt();
}

QString GlobalSetting::getUserName()
{
    if (!m_pSettings)
        return QString();
    return m_pSettings->value("Login/userName","").toString();
}

void GlobalSetting::setUserName(const QString& userName)
{
    if (m_pSettings)
        m_pSettings->setValue("Login/userName",userName);
}

QString GlobalSetting::getPWD()
{
    if (!m_pSettings)
        return QString();
    QString pwd = m_pSettings->value("Login/pwd","").toString();
    auto data = pwd.toLatin1();
    return data;
}

void GlobalSetting::setPWD(const QString& pwd)
{
    auto data = pwd.toLatin1();
    if (m_pSettings)
        m_pSettings->setValue("Login/pwd",data);
}

bool GlobalSetting::getRememberPWD()
{
    if (!m_pSettings)
        return false;
    return m_pSettings->value("Login/rememberPWD",false).toBool();
}

void GlobalSetting::setRememberPWD(bool b)
{
    if(m_pSettings)
        m_pSettings->setValue("Login/rememberPWD",b);
}

bool GlobalSetting::getAutoLogin()
{
    if (!m_pSettings)
        return false;
    return m_pSettings->value("Login/autoLogin",false).toBool();
}

void GlobalSetting::setAutoLogin(bool b)
{
    if(m_pSettings)
        m_pSettings->setValue("Login/autoLogin",b);
}

void GlobalSetting::setRobotBot(bool open)
{
    if (m_pSettings)
        m_pSettings->setValue("Setting/robotBot", open);
}

bool GlobalSetting::getRobotBot()
{
    if (!m_pSettings)
        return true;
    return m_pSettings->value("Setting/robotBot", true).toBool();
}

void GlobalSetting::setSoundBot(bool open)
{
    if (m_pSettings)
        m_pSettings->setValue("Setting/soundBot", open);
}

bool GlobalSetting::getSoundBot()
{
    if (!m_pSettings)
        return true;
    return m_pSettings->value("Setting/soundBot", true).toBool();
}

void GlobalSetting::setSpeechBk(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/SpeechBk", bk);
}

int GlobalSetting::getSpeechBk()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/SpeechBk", 1).toInt();
}

void GlobalSetting::setSpeechOg(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/SpeechOg", bk);
}

int GlobalSetting::getSpeechOg()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/SpeechOg", 1).toInt();
}

QColor GlobalSetting::getSpeechBkColor()
{
    switch (getSpeechBk())
    {
        case 1:
            return QColor{ 19, 19, 19, 204 };

        case 2:
            return QColor{ 155, 155, 155, 204 };
    }

    return QColor{ 19, 19, 19, 204 };
}

QColor GlobalSetting::getSpeechOgColor()
{
    switch (getSpeechOg())
    {
        case 1:
            return QColor{ 19, 19, 19, 255 };

        case 2:
            return QColor{ 0, 215, 244, 255 };

        case 3:
            return QColor{ 0, 169, 255, 255 };

        case 4:
            return QColor{ 107, 251, 206, 255 };
    }

    return QColor{ 19, 19, 19, 255 };
}

void GlobalSetting::setTransBk(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/TransBk", bk);
}

int GlobalSetting::getTransBk()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/TransBk", 1).toInt();
}

void GlobalSetting::setTransOg(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/TransOg", bk);
}

int GlobalSetting::getTransOg()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/TransOg", 1).toInt();
}

QColor GlobalSetting::getTransBkColor()
{
    switch (getTransBk())
    {
    case 1:
        return QColor{ 19, 19, 19, 204 };

    case 2:
        return QColor{ 155, 155, 155, 204 };
    }

    return QColor{ 19, 19, 19, 204 };
}

QColor GlobalSetting::getTransOgColor()
{
    switch (getTransTl())
    {
    case 1:
        return QColor{ 19, 19, 19, 255 };

    case 2:
        return QColor{ 0, 215, 244, 255 };

    case 3:
        return QColor{ 0, 169, 255, 255 };

    case 4:
        return QColor{ 107, 251, 206, 255 };
    }

    return QColor{ 19, 19, 19, 255 };
}

QColor GlobalSetting::getTransColor()
{
    switch (getTransTl())
    {
    case 1:
        return QColor{ 19, 19, 19, 255 };

    case 2:
        return QColor{ 0, 215, 244, 255 };

    case 3:
        return QColor{ 0, 169, 255, 255 };

    case 4:
        return QColor{ 107, 251, 206, 255 };
    }

    return QColor{ 19, 19, 19, 255 };
}

void GlobalSetting::setTransTl(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/TransTl", bk);
}

int GlobalSetting::getTransTl()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/TransTl", 1).toInt();
}

void GlobalSetting::setTransTT(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/TransTT", bk);
}

int GlobalSetting::getTransTT()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/TransTT", 1).toInt();
}

void GlobalSetting::setTransSD(int bk)
{
    if (m_pSettings)
        m_pSettings->setValue("Color/TransSD", bk);
}

int GlobalSetting::getTransSD()
{
    if (!m_pSettings)
        return 1;
    return m_pSettings->value("Color/TransSD", 1).toInt();
}

QAudioDeviceInfo& GlobalSetting::MicDeviceInfo()
{
    return m_micDeviceInfo;
}

QAudioDeviceInfo& GlobalSetting::SpeakerDeviceInfo()
{
    return m_speakerDeviceInfo;
}

QAudioDeviceInfo& GlobalSetting::MonitorDeviceInfo()
{
    return m_monitorDeviceInfo;
}



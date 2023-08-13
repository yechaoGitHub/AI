#include "GlobalSetting.h"
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
    m_pSettings = new QSettings(path, QSettings::IniFormat);
    if (m_pSettings)
        return true;
    return false;
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




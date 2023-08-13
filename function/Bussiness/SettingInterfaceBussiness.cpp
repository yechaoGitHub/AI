#include "SettingInterfaceBussiness.h"
#include "base/HttpClient.h"
#include "base/GlobalSetting.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>


SettingInterfaceBussiness* SettingInterfaceBussiness::_instance = nullptr;
QMutex  g_mutex;
SettingInterfaceBussiness::SettingInterfaceBussiness(QObject *parent)
    : QObject(parent)
{}

SettingInterfaceBussiness::~SettingInterfaceBussiness()
{}

SettingInterfaceBussiness* SettingInterfaceBussiness::getInstance()
{
    if (!_instance) {
        QMutexLocker lk(&g_mutex);
        if (!_instance) {
            _instance = new SettingInterfaceBussiness(nullptr);
        }
    }
    return _instance;
}

void SettingInterfaceBussiness::getUserInfoReq()
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    stru_UserInfo user_info;
    HttpClient client(QString("%1/api/setting/getUserInfo").arg(url));
    client.success([=](const QString& response) {
        int i = 0;
        });
    client.timeout([=]() {
        qDebug() << "getUserInfoReq timeout";
        emit sig_getUserInfoReplay(false, netCode::TimeOut,"查询个人信息超时", user_info);
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getUserInfoReq error code=" << code;
        emit sig_getUserInfoReplay(false, netCode::ServerErr, "查询个人信息请求失败", user_info);
        });
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).timeout(10).post();

}
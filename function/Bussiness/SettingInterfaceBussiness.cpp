#include "SettingInterfaceBussiness.h"
#include "AiSound.h"
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

void SettingInterfaceBussiness::paraseHttpResponse(httpReqType req_type, const QString& response)
{
    QJsonParseError err_rpt;
    auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError) {
        return;
    }

    if (req_type == httpReqType::UserInfo_Req){
        stru_UserInfo user_info;
        if (json["code"].toInt() == 200) {
            user_info.userId = json["data"]["userId"].toInt();
            user_info.phoneId = json["data"]["mobileNumber"].toString();
            user_info.userName = json["data"]["username"].toString();
            user_info.balance = json["data"]["userPackageVO"]["balance"].toDouble();
        }
        emit sig_getUserInfoReplay(true, netCode::Success, "", user_info);
    }
    else if (req_type == httpReqType::InviteUser_Req) {
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::InviteUser_Req, true, "");
        }
        else {
            emit sig_common_replay(httpReqType::InviteUser_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::Remove_Team || req_type == httpReqType::Feedback) {
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(req_type, true, "");
        }
        else {
            emit sig_common_replay(req_type, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::Search_Team) {
        if (json["code"].toInt() == 200) {
            QVector<struc_teamInfo>   team_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    struc_teamInfo team_info;
                    QJsonObject json_recode = it.toObject();
                    qint64 timestamp = (qint64)json_recode["initTime"].toDouble();
                    QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamp);
                    team_info.join_time = time.toString("yyyy-MM-dd hh:mm:ss");
                    team_info.username = json_recode["userName"].toString();
                    team_info.userId = json_recode["userId"].toInt();
                    team_info.status = json_recode["status"].toInt();
                    team_info.credits_used = json_recode["cost"].toDouble();
                    team_list.push_back(std::move(team_info));
                }
            }
            int pages = json["data"]["pages"].toInt();
            int cur_page = json["data"]["current"].toInt();
            int total = json["data"]["total"].toInt();
            emit sig_searchTeam_replay(pages,total, cur_page, team_list);
        }
        else {
            emit sig_common_replay(httpReqType::Search_Team, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::ChatBot_req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_ChatbotInfo>   chatbot_list;
            if (json["data"].isArray()) {
                for (auto it : json["data"].toArray()) {
                    strc_ChatbotInfo chatbot_info;
                    QJsonObject json_recode = it.toObject();
                    chatbot_info.desc = json_recode["description"].toString();
                    chatbot_info.initialMessage = json_recode["initialMessage"].toString();
                    chatbot_info.name = json_recode["name"].toString();
                    chatbot_info.type = json_recode["type"].toInt();
                    chatbot_info.typeName = json_recode["typeName"].toString();
                    chatbot_info.isRecommend = json_recode["isRecommend"].toBool();
                    chatbot_list.push_back(std::move(chatbot_info));
                }
            }
            emit sig_getChatBotListReplay(true, json["code"].toInt(), "", chatbot_list);
        }
        else {
            emit sig_common_replay(req_type, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::ChatHistory_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_ChatHistory>   chat_history_list;
            if (json["data"].isArray()) {
                for (auto it : json["data"].toArray()) {
                    strc_ChatHistory chat_history;
                    QJsonObject json_recode = it.toObject();
                    chat_history.chatHistoryId = json_recode["chatHistoryId"].toInt();
                    chat_history.chatType = json_recode["chatType"].toInt();
                    chat_history.content = json_recode["content"].toString();
                    chat_history.initTime = json_recode["initTime"].toDouble();
                    chat_history.receiverId = json_recode["receiverId"].toInt();
                    chat_history.senderId = json_recode["senderId"].toDouble();
                    chat_history_list.push_back(std::move(chat_history));
                }
            }

            emit sig_chatHistoryReplay(true, 200, "", chat_history_list);
        }
        else {
            emit sig_common_replay(httpReqType::ChatHistory_Req, false, json["msg"].toString());
        }
    }
}

void SettingInterfaceBussiness::getUserInfoReq()
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    HttpClient client(QString("%1/api/setting/getUserInfo").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::UserInfo_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getUserInfoReq timeout";
        emit sig_getUserInfoReplay(false, netCode::TimeOut, "查询个人信息超时", stru_UserInfo{});
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getUserInfoReq error code=" << code;
        emit sig_getUserInfoReplay(false, netCode::ServerErr, "查询个人信息请求失败", stru_UserInfo{});
        });
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).timeout(10).post();
}

void SettingInterfaceBussiness::inviteUserJoinReq(const QString& username)
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("inviteUserName", username);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/teams/invite").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::InviteUser_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "inviteUserJoinReq timeout";
        emit sig_common_replay(httpReqType::InviteUser_Req, false, "邀请用户超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "inviteUserJoinReq error code=" << code;
        emit sig_common_replay(httpReqType::InviteUser_Req, false, "邀请用户请求失败");
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getTeamRecordReq(int page, int pageSize, const QString& search)
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", page);
    dataobj.insert("pageSize", pageSize);
    dataobj.insert("searchText", search);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/teams/getMembers").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::Search_Team, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::Search_Team, false, "查询团队成员超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::Search_Team, false, "查询团队成员超时");
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::removeTeamReq(qint64 userId)
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("userId", userId);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/teams/removeUser").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::Remove_Team, response);
        });
    client.timeout([=]() {
        qDebug() << "removeTeamReq timeout";
        emit sig_common_replay(httpReqType::Remove_Team, false, "移除用户超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "removeTeamReq error code=" << code;
        emit sig_common_replay(httpReqType::Remove_Team, false, "移除用户请求失败");
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::feedBackReq(const QString& msg)
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("content", msg);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/setting/feedback").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::Feedback, response);
        });
    client.timeout([=]() {
        qDebug() << "feedBackReq timeout";
        emit sig_common_replay(httpReqType::Feedback, false, "反馈超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "feedBackReq error code=" << code;
        emit sig_common_replay(httpReqType::Feedback, false, "反馈请求失败");
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getCharBotListReq()
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    HttpClient client(QString("%1/api/config/getChatbotTemplateList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::ChatBot_req, response);
        });
    client.timeout([=]() {
        qDebug() << "getCharBotListReq timeout";
        emit sig_common_replay(httpReqType::ChatBot_req, false, "获取ChatBot超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getCharBotListReq error code=" << code;
        emit sig_common_replay(httpReqType::ChatBot_req, false, "获取ChatBot失败");
        });
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).timeout(10).post();
}

void SettingInterfaceBussiness::getCharHistoryReq(int type ,int page, const QString& search, int pageSize)
{
    QString token = SETTING.getToken();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", page);
    dataobj.insert("chatType", type);
    dataobj.insert("pageSize", pageSize);
    dataobj.insert("searchText", search);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/setting/getChatHistory").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::ChatHistory_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::ChatHistory_Req, false, "查询聊天记录超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::ChatHistory_Req, false, "查询聊天记录失败");
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}
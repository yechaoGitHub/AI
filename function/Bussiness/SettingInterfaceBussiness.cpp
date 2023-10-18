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
            _instance = new SettingInterfaceBussiness();
            _instance->initThread();
        }
    }
    return _instance;
}

void SettingInterfaceBussiness::initThread()
{
    _thread = new QThread();
    _instance->moveToThread(_thread);
    _thread->start();

    _thread->setObjectName("http_thread");
}

void SettingInterfaceBussiness::uninitialize()
{
    if (_thread) {
        _thread->quit();
        _thread->wait();
        delete _thread;
    }
}

void SettingInterfaceBussiness::paraseHttpResponse(httpReqType req_type, const QString& response)
{
    QMutexLocker lk(&_mutex);
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
            user_info.balance = json["data"]["balance"].toDouble();
        }
        emit sig_getUserInfoReplay(true, netCode::Success, "", user_info);
    }
    else if (req_type == httpReqType::InviteUser_Req) {
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::InviteUser_Req, true, tr("invite Success"));
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
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_ChatbotInfo chatbot_info;
                    QJsonObject json_recode = it.toObject();
                    chatbot_info.desc = json_recode["description"].toString();
                    chatbot_info.initialMessage = json_recode["initialMessage"].toString();
                    chatbot_info.name = json_recode["name"].toString();
                    chatbot_info.id = json_recode["id"].toInt();
                    chatbot_info.type = json_recode["type"].toInt();
                    chatbot_info.typeName = json_recode["typeName"].toString();
                    chatbot_info.isRecommend = json_recode["isRecommend"].toBool();
                    chatbot_list.push_back(std::move(chatbot_info));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_getChatBotListReplay(true, json["code"].toInt(), page_info, chatbot_list);
        }
        else {
            emit sig_common_replay(req_type, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::ChatHistory_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_ChatHistory>   chat_history_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_ChatHistory chat_history;
                    QJsonObject json_recode = it.toObject();
                    chat_history.chatHistoryId = json_recode["id"].toInt();
                    chat_history.conversationId = json_recode["conversationId"].toString();
                    chat_history.chatType = json_recode["chatType"].toInt();
                    chat_history.content = json_recode["content"].toString();
                    QDateTime date = QDateTime::fromMSecsSinceEpoch((qint64)(json_recode["updateTime"].toDouble()));
                    chat_history.initTime = date.toString("yyyy-MM-dd hh:mm:ss");
                    chat_history.receiverId = json_recode["receiverId"].toInt();
                    chat_history.senderId = json_recode["senderId"].toDouble();
                    chat_history.synopsis = json_recode["synopsis"].toString();
                    chat_history.templateName = json_recode["templateName"].toString();
                    chat_history.chatName = json_recode["chatName"].toString();
                    chat_history_list.push_back(std::move(chat_history));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_chatHistoryReplay(true, 200, page_info, chat_history_list);
        }
        else {
            emit sig_common_replay(httpReqType::ChatHistory_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::Filter_req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_SoundFilter>  filter_list;
            if (json["data"]["labelList"].isArray()) {
                for (auto it : json["data"]["labelList"].toArray()) {
                    strc_SoundFilter sound_filter;
                    QJsonObject json_recode = it.toObject();
                    sound_filter.is_label = true;
                    sound_filter.id = json_recode["id"].toInt();
                    sound_filter.value = json_recode["value"].toString();
                    filter_list.push_back(std::move(sound_filter));
                }
            }
            if (json["data"]["languageList"].isArray()) {
                for (auto it : json["data"]["languageList"].toArray()) {
                    strc_SoundFilter sound_filter;
                    QJsonObject json_recode = it.toObject();
                    sound_filter.id = json_recode["id"].toInt();
                    sound_filter.value = json_recode["value"].toString();
                    filter_list.push_back(std::move(sound_filter));
                }
            }

            emit sig_soundFilterReplay(true, 200, "", filter_list);
        }
        else {
            emit sig_common_replay(req_type, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::SoundLib_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_SoundLib>   sound_lib_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_SoundLib sound_lib;
                    QJsonObject json_recode = it.toObject();
                    sound_lib.description = json_recode["description"].toString();
                    sound_lib.gender = json_recode["gender"].toInt();
                    sound_lib.label = json_recode["label"].toInt();
                    sound_lib.language = json_recode["language"].toInt();
                    sound_lib.source = json_recode["source"].toInt();
                    sound_lib.voiceLibId = json_recode["voiceLibId"].toDouble();
                    sound_lib.voiceName = json_recode["voiceName"].toString();
                    sound_lib.clonedByCount = json_recode["clonedByCount"].toInt();
                    sound_lib_list.push_back(std::move(sound_lib));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_soundLibReplay(true, 200, page_info, sound_lib_list);
        }
        else {
            emit sig_common_replay(httpReqType::SoundLib_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::VoiceList_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_MyVoice>   voice_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_MyVoice voice;
                    QJsonObject json_recode = it.toObject();
                    voice.description = json_recode["description"].toString();
                    voice.voiceId = json_recode["voiceId"].toInt();
                    voice.voiceName = json_recode["voiceName"].toString();
                    voice.voiceType = json_recode["voiceType"].toInt();
                    voice.gender = json_recode["gender"].toInt();
                    voice.language = json_recode["language"].toInt();
                    voice_list.push_back(std::move(voice));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_myVoiceListReplay(true, 200, page_info, voice_list);
        }
        else {
            emit sig_common_replay(httpReqType::VoiceList_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == httpReqType::ChatBotType_Req) {
        if (json["code"].toInt() == 200) {
            QMap<int, QString> chatbot_map;
            if (json["data"]["typeList"].isArray()) {
                for (auto it : json["data"]["typeList"].toArray()) {
                    strc_MyVoice voice;
                    QJsonObject json_recode = it.toObject();
                    QString value = json_recode["value"].toString();
                    int id = json_recode["id"].toInt();
                    chatbot_map[id] = value;
                }
            }
            emit sig_chatBotListReplay(true,httpReqType::ChatBotType_Req, chatbot_map);
        }
        else {
            emit sig_common_replay(httpReqType::ChatBotType_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == TransHistory_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_transHistory>   chat_history_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_transHistory chat_history;
                    QJsonObject json_recode = it.toObject();
                    chat_history.id = json_recode["id"].toInt();
                    chat_history.transName = json_recode["transName"].toString();
                    chat_history.transTypeName = json_recode["transTypeName"].toString();
                    chat_history_list.push_back(std::move(chat_history));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_transHistoryReplay(true, 200, page_info, chat_history_list);
        }
        else {
            emit sig_common_replay(httpReqType::TransHistory_Req, false, json["msg"].toString());
        }
    }
    else if (req_type == ChatRecord_Req) {
        if (json["code"].toInt() == 200) {
            QVector<strc_chatRecord>   chat_record_list;
            if (json["data"]["records"].isArray()) {
                for (auto it : json["data"]["records"].toArray()) {
                    strc_chatRecord chat_record;
                    QJsonObject json_recode = it.toObject();
                    chat_record.role = json_recode["role"].toString();
                    chat_record.content = json_recode["content"].toString();
                    chat_record_list.push_back(std::move(chat_record));
                }
            }

            strc_PageInfo page_info;
            page_info.total_size = json["data"]["total"].toInt();
            page_info.cur_page = json["data"]["current"].toInt();
            page_info.page_size = json["data"]["size"].toInt();
            page_info.total_pages = json["data"]["pages"].toInt();

            emit sig_chatRecordReplay(true, 200, page_info, chat_record_list);
        }
        else {
            emit sig_common_replay(httpReqType::TransHistory_Req, false, json["msg"].toString());
        }
    }
}

void SettingInterfaceBussiness::getUserInfoReq()
{
    QMetaObject::invokeMethod(this, "_getUserInfoReq");
}

void SettingInterfaceBussiness::_getUserInfoReq()
{
    QString token = AiSound::GetInstance().Token();
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

void SettingInterfaceBussiness::getGlobalInfo()
{
    QMetaObject::invokeMethod(this, "getGlobalInfo");
}

void SettingInterfaceBussiness::_getGlobalInfo()
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    HttpClient client(QString("%1/api/config/getGlobalConfig").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            qDebug() << response;
            return;
        }
        if (json["code"].toInt() == 200) {
            SETTING.setRechargeUrl(json["data"]["websiteRechargeUrl"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "_getGlobalInfo timeout";
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "_getGlobalInfo error code=" << code;
        });
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).timeout(10).post();
}

void SettingInterfaceBussiness::inviteUserJoinReq(const QString& username)
{
    QString token = AiSound::GetInstance().Token();
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
    QMetaObject::invokeMethod(this, "_getTeamRecordReq", Q_ARG(int, page), Q_ARG(int, pageSize), Q_ARG(QString, search));
}

void SettingInterfaceBussiness::_getTeamRecordReq(int page, int pageSize, const QString& search)
{
    QString token = AiSound::GetInstance().Token();
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

void SettingInterfaceBussiness::_removeTeamReq(qint64 userId)
{
    QString token = AiSound::GetInstance().Token();
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
        emit sig_common_replay(httpReqType::Remove_Team, false, tr("remove user timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "removeTeamReq error code=" << code;
        emit sig_common_replay(httpReqType::Remove_Team, false, tr("remove user fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::removeTeamReq(qint64 userId)
{
    QMetaObject::invokeMethod(this, "_removeTeamReq", Q_ARG(qint64, userId));
}

void SettingInterfaceBussiness::feedBackReq(const QString& msg)
{
    QString token = AiSound::GetInstance().Token();
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

void SettingInterfaceBussiness::getChatBotType()
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    HttpClient client(QString("%1/api/config/getChatbotTempTypeFilter").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::ChatBotType_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::ChatBotType_Req, false, tr("get chatbot type timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::ChatBotType_Req, false, tr("get chatbot type fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).timeout(10).post();
}

void SettingInterfaceBussiness::getCharBotListReq(int page, int page_size,int type)
{
    QMetaObject::invokeMethod(this, "_getCharBotListReq",Q_ARG(int, page), Q_ARG(int, page_size), Q_ARG(int, type));
}

void SettingInterfaceBussiness::_getCharBotListReq(int page, int page_size,int type)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", page);
    dataobj.insert("pageSize", page_size);
    if (type >= 0) {
        dataobj.insert("type", type);
    }
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

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
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getChatHistoryReq(int page, const QString& search, int pageSize)
{
    QString token = AiSound::GetInstance().Token();
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

    HttpClient client(QString("%1/api/chatbot/getChatbotHistoryList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::ChatHistory_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::ChatHistory_Req, false, tr("search chatbot history timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::ChatHistory_Req, false, tr("search chatbot history fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::delChatHsitory(const QStringList& chatId_list)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }
    QJsonObject dataobj;
    QJsonArray array;
    for (auto it : chatId_list) {
        array.append(it.toInt());
    }
    dataobj.insert("idList", array);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/chatbot/deleteChatHistory").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::Del_ChatHistory, false, tr("del chat history"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::Del_ChatHistory, true, tr("del Success"));
        }
        else {
            emit sig_common_replay(httpReqType::Del_ChatHistory, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "delChatHsitory timeout";
        emit sig_common_replay(httpReqType::Del_ChatHistory, false, tr("del chat history timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "delChatHsitory error code=" << code;
        emit sig_common_replay(httpReqType::Del_ChatHistory, false, tr("del chat history fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getFilterListReq()
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    HttpClient client(QString("%1/api/voice/library/getFilterList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::Filter_req, response);
        });
    client.timeout([=]() {
        qDebug() << "getFilterListReq timeout";
        emit sig_common_replay(httpReqType::Filter_req, false, "获取声音列表超时");
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getFilterListReq error code=" << code;
        emit sig_common_replay(httpReqType::Filter_req, false, "获取声音列表失败");
        });
    client.header("Content-Type", "application/x-www-form-urlencoded").header("access_token", token).timeout(10).post();
}

void SettingInterfaceBussiness::getSoundLIbReq(int pageNo, int pageSize, strc_SoundType sound_type)
{
    QMetaObject::invokeMethod(this, "_getSoundLIbReq", Q_ARG(int, pageNo), Q_ARG(int, pageSize), Q_ARG(strc_SoundType, sound_type));
}

void SettingInterfaceBussiness::_getSoundLIbReq(int pageNo, int pageSize, strc_SoundType sound_type)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", pageNo);
    dataobj.insert("pageSize", pageSize);
    if (sound_type.gender != 0) {
        dataobj.insert("gender", sound_type.gender);
    }
    if (sound_type.label != 0) {
        dataobj.insert("label", sound_type.label);
    }
    if (sound_type.language != 0) {
        dataobj.insert("language", sound_type.language);
    }
    if (sound_type.source != 0) {
        dataobj.insert("source", sound_type.source);
    }

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/library/getList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::SoundLib_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::SoundLib_Req, false, tr("获取声音列表超时"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::SoundLib_Req, false, tr("查询声音列表失败"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::addMyVoice(int libId, const QString& voiceName)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("voiceLibId", libId);
    dataobj.insert("voiceLibName", voiceName);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/library/addVoice").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::AddVoice, false, tr("add voice fail"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::AddVoice, true, tr("add Success"));
        }
        else {
            emit sig_common_replay(httpReqType::AddVoice, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "feedBackReq timeout";
        emit sig_common_replay(httpReqType::AddVoice, false, tr("add voice timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "feedBackReq error code=" << code;
        emit sig_common_replay(httpReqType::AddVoice, false, tr("add voice fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getVoiceListReq(int cur_page, int page_size)
{
    QMetaObject::invokeMethod(this, "_getVoiceListReq", Q_ARG(int, cur_page), Q_ARG(int, page_size));
}

void SettingInterfaceBussiness::_getVoiceListReq(int cur_page, int page_size)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", cur_page);
    dataobj.insert("pageSize", page_size);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/myVoice/getList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::VoiceList_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getVoiceListReq timeout";
        emit sig_common_replay(httpReqType::VoiceList_Req, false, tr("get voice list timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getVoiceListReq error code=" << code;
        emit sig_common_replay(httpReqType::VoiceList_Req, false, tr("get voice list fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::delVoiceReq(int voiceId)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("voiceId", voiceId);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/myVoice/delete").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::DelVoice_Req, false, tr("delete voice fail"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::DelVoice_Req, true, tr("delete voice Success"));
        }
        else {
            emit sig_common_replay(httpReqType::DelVoice_Req, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "delVoiceReq timeout";
        emit sig_common_replay(httpReqType::DelVoice_Req, false, tr("delete voice timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "delVoiceReq error code=" << code;
        emit sig_common_replay(httpReqType::DelVoice_Req, false, tr("delete voice fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getVoiceUrlReq(int voiceId,bool my_voice)
{
    if (my_voice) {
        QMetaObject::invokeMethod(this, "_getVoiceUrlReq", Q_ARG(int, voiceId));
    }
    else {
        QMetaObject::invokeMethod(this, "_getVoiceLibUrlReq", Q_ARG(int, voiceId));
    }
}

void SettingInterfaceBussiness::_getVoiceUrlReq(int voiceId)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("voiceId", voiceId);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/myVoice/getVoiceUrl").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::GetVoiceUrl_Req, false, tr("get voice url timeout"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::GetVoiceUrl_Req, true, json["data"]["voiceUrl"].toString());
        }
        else {
            emit sig_common_replay(httpReqType::GetVoiceUrl_Req, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "getVoiceListReq timeout";
        emit sig_common_replay(httpReqType::GetVoiceUrl_Req, false, tr("get voice url timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getVoiceListReq error code=" << code;
        emit sig_common_replay(httpReqType::GetVoiceUrl_Req, false, tr("get voice url fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::_getVoiceLibUrlReq(int voiceId)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("voiceId", voiceId);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/library/getVoiceUrl").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::GetVoiceLib_Req, false, tr("get voice url timeout"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::GetVoiceLib_Req, true, json["data"]["voiceUrl"].toString());
        }
        else {
            emit sig_common_replay(httpReqType::GetVoiceLib_Req, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "getVoiceListReq timeout";
        emit sig_common_replay(httpReqType::GetVoiceLib_Req, false, tr("get voice url timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getVoiceListReq error code=" << code;
        emit sig_common_replay(httpReqType::GetVoiceLib_Req, false, tr("get voice url fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::editMyVoice(int libId, const QString& voiceName, const QString& desc)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("voiceId", libId);
    dataobj.insert("voiceName", voiceName);
    dataobj.insert("description", desc);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/voice/myVoice/edit").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::EditVoice, false, tr("edit voice fail"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::EditVoice, true, tr("edit Success"));
        }
        else {
            emit sig_common_replay(httpReqType::EditVoice, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "editMyVoice timeout";
        emit sig_common_replay(httpReqType::EditVoice, false, tr("add voice timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "editMyVoice error code=" << code;
        emit sig_common_replay(httpReqType::AddVoice, false, tr("edit voice fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getTransHistory(int page_size, int pageNo, const QString& search)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", pageNo);
    dataobj.insert("pageSize", page_size);
    dataobj.insert("searchText", search);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/translate/getTransHistoryList").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::TransHistory_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "getTeamRecordReq timeout";
        emit sig_common_replay(httpReqType::TransHistory_Req, false, tr("search Trans history timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "getTeamRecordReq error code=" << code;
        emit sig_common_replay(httpReqType::TransHistory_Req, false, tr("search Trans history fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::delTransId(int id)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    QJsonArray array;
   array.append(id);
   dataobj.insert("idList", array);
   QJsonDocument document;
   document.setObject(dataobj);
   QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/translate/deletetTransHistory").arg(url));
    client.success([=](const QString& response) {
        QJsonParseError err_rpt;
        auto json = QJsonDocument::fromJson(response.toUtf8(), &err_rpt);
        if (err_rpt.error != QJsonParseError::NoError) {
            emit sig_common_replay(httpReqType::DelTrans_Req, false, tr("delete trans fail"));
            return;
        }
        if (json["code"].toInt() == 200) {
            emit sig_common_replay(httpReqType::DelTrans_Req, true, tr("delete trans Success"));
        }
        else {
            emit sig_common_replay(httpReqType::DelTrans_Req, false, json["msg"].toString());
        }
        });
    client.timeout([=]() {
        qDebug() << "delVoiceReq timeout";
        emit sig_common_replay(httpReqType::DelTrans_Req, false, tr("delete trans timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "delVoiceReq error code=" << code;
        emit sig_common_replay(httpReqType::DelTrans_Req, false, tr("delete voice fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}

void SettingInterfaceBussiness::getChatRecord(int page_size, int pageNo, const QString& chatId)
{
    QMetaObject::invokeMethod(this, "_getChatRecord", Q_ARG(int, page_size), Q_ARG(int, pageNo), Q_ARG(QString, chatId));
}

void SettingInterfaceBussiness::_getChatRecord(int page_size, int pageNo, const QString& chatId)
{
    QString token = AiSound::GetInstance().Token();
    QString url = SETTING.getHostAddress();
    if (token.isEmpty() || url.isEmpty()) {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("pageNo", pageNo);
    dataobj.insert("pageSize", page_size);
    dataobj.insert("conversationId", chatId);
    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray jsonValue = document.toJson(QJsonDocument::Compact);

    HttpClient client(QString("%1/api/chatbot/load").arg(url));
    client.success([=](const QString& response) {
        paraseHttpResponse(httpReqType::ChatRecord_Req, response);
        });
    client.timeout([=]() {
        qDebug() << "_getChatRecord timeout";
        emit sig_common_replay(httpReqType::ChatRecord_Req, false, tr("get chatbot record timeout"));
        });
    client.fail([=](const QString& response, int code) {
        qDebug() << "_getChatRecord error code=" << code;
        emit sig_common_replay(httpReqType::ChatRecord_Req, false, tr("get chatbot record fail"));
        });
    client.header("Content-Type", "application/json").header("access_token", token).json(jsonValue).timeout(10).post();
}
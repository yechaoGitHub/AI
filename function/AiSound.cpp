#include "AiSound.h"

#include "WTranslationSelect.h"
#include "WTransaltionMain.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QAudioInput>
#include <QDesktopWidget>

#include <Windows.h>
#include <thread>


AiSound AiSound::INSTANCE;

AiSound::AiSound()
{

}

AiSound::~AiSound()
{
}

void AiSound::Initialize()
{
    connect(&_networkAccess, &QNetworkAccessManager::finished, this, &AiSound::HttpCallbackDispatch);

    _wLoginFrame = new WLoginUI{};
    _wTranslationSelect = new WTranslationSelect{};
    _wTranslationMain = new WTransaltionMain{};
    _robotNaviga = new WRobotNavigation(nullptr);
    connect(_robotNaviga,&WRobotNavigation::sig_robot_clicked,this, &AiSound::slot_robot_nv_clicked);

    _robot_chat = new RobotChatMainUI();
    _speech_ui = new WSpeechGenerationUi();
    _set_main = new WSettingMainUi();

    _wTip = new WTip{};
    connect(_wTip, &WTip::tipEnd, this, &AiSound::NextMessage);

    _translation.Initialize();
    _voiceCompositor.Initialize();
    _chatBot.Initialize();
}

void AiSound::Uninitialize()
{
    //_networkAccess.

    _translation.Uninitialize();
    _voiceCompositor.Uninitialize();
    _chatBot.Uninitialize();
}

AiSound& AiSound::GetInstance()
{
    return INSTANCE;
}

void AiSound::PasswordLogin(const QString& userName, const QString& password, PasswordLoginCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/user/loginByPwd" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    QJsonObject dataobj;

   dataobj.insert("password", password);
   dataobj.insert("username", userName);

   QJsonDocument document;
   document.setObject(dataobj);
   QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    auto packet = new HttpCallbackPacket<PasswordLoginCallbackType>();
    packet->type = httpPasswordLogin;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, byte_array);
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::GetVerifyCode(GetVerifyCodeCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/common/getImgVerifyCode" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    QJsonObject dataobj;
    dataobj.insert("moduleType", "login");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    auto packet = new HttpCallbackPacket<GetVerifyCodeCallbackType>();
    packet->type = httpGetVerifyCode;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, byte_array);
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::Register(const QString& userName, const QString& password, const QString& dialingCode, const QString& phoneNumber, const QString& verifyCode, RegisterCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/user/register"});
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    QJsonObject dataobj;
    dataobj.insert("moduleType", "register");
    dataobj.insert("username", userName);
    dataobj.insert("password", password);
    dataobj.insert("rePassword", password);
    dataobj.insert("dialingCode", dialingCode);
    dataobj.insert("mobileNumber", phoneNumber);
    dataobj.insert("verifyCode", verifyCode);

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    auto packet = new HttpCallbackPacket<RegisterCallbackType>();
    packet->type = httpRegister;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, byte_array);
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::SendVerifyCode(const QString& dCode, const QString& mobileNumber, const QString& verifyCode, const QString& uuid, SendVerifyCodeCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/user/sendSmsVerifyCode" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    QJsonObject dataobj;
    dataobj.insert("moduleType", "login");
    dataobj.insert("mobileNumber", mobileNumber);
    dataobj.insert("dialingCode", dCode);
    dataobj.insert("imgVerifyCode", verifyCode);
    dataobj.insert("uuid", uuid);

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    auto packet = new HttpCallbackPacket<SendVerifyCodeCallbackType>();
    packet->type = httpSendVerifyCode;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, byte_array);
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::GetTranslationSrourceList(GetTranslationSourceListCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/config/getSourceLanguaueList" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    request.setRawHeader("access_token", _token.toUtf8());

    auto packet = new HttpCallbackPacket<GetTranslationSourceListCallbackType>();
    packet->type = httpGetTranslationSource;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, QByteArray{});
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::GetTranslationDestList(GetTranslationDestListCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/config/getTargetLanguaueList" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    request.setRawHeader("access_token", _token.toUtf8());

    auto packet = new HttpCallbackPacket<GetTranslationSourceListCallbackType>();
    packet->type = httpGetTranslationDest;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, QByteArray{});
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::GetVoiceSpeaker(GetVoiceSpeakerCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/api/config/getSpeakers" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    request.setRawHeader("access_token", _token.toUtf8());

    auto packet = new HttpCallbackPacket<GetVoiceSpeakerCallbackType>();
    packet->type = httpGetVoiceSpeakerCallback;
    packet->callback = callback;

    auto reply = _networkAccess.post(request, QByteArray{});
    reply->setUserData(Qt::UserRole, packet);
}

void AiSound::ShowLoginFrame()
{
    _wLoginFrame->show();
}

void AiSound::ShowTranslationWindow()
{
    //_wTranslationSelect->show();
    _wLoginFrame->close();
    QDesktopWidget* deskWgt = QApplication::desktop();
    QRect availableRect = deskWgt->availableGeometry();
    _robotNaviga->move(availableRect.width()- _robotNaviga->width(), availableRect.height() - _robotNaviga->height());
    _robotNaviga->show();
}

void AiSound::slot_robot_nv_clicked(Navig_Type type)
{
    if (AiFunctionRunning())
    {
        return;
    }

    if (type == Navig_Type::Chat) {
        if (_robot_chat->isHidden())
        {
            _robot_chat->show();
        }
    }
    else if (type == Navig_Type::Voice) {
        if (_wTranslationSelect->isHidden())
        {
            _wTranslationSelect->SetFunctionType(FunctionType::Translation);
            _wTranslationSelect->show();
        }
    }
    else if (type == Navig_Type::Speech) {
        if (_wTranslationSelect->isHidden())
        {
            _wTranslationSelect->SetFunctionType(FunctionType::VoiceCompositor);
            _wTranslationSelect->show();
        }
    }
    else if (type == Navig_Type::System_Set) {
        _set_main->show();
    }
}

void AiSound::ShowTranslationMainWindow(const TranslationLanguage& srcLan, const TranslationLanguage& destLan)
{
    _wTranslationMain->SetLanguage(srcLan, destLan);
    _wTranslationMain->show();
}

void AiSound::ShowVoiceCompositorMainWindow(const TranslationLanguage& srcLan, const TranslationLanguage& destLan)
{
    _speech_ui->SetLanguage(srcLan, destLan);
    _speech_ui->show();
}

void AiSound::ShowTip( const QString& msg)
{
    if (_wTip->isHidden())
    {
        _wTip->SetMessage(msg);
        _wTip->show();
    }
    else
    {
        _ltMsg.push_back(msg);
    }
}

Translation& AiSound::GetTranslation()
{
    return _translation;
}

VoiceCompositor& AiSound::GetVoiceCompositor()
{
    return _voiceCompositor;
}

ChatBot& AiSound::GetChatBot()
{
    return _chatBot;
}

const QString& AiSound::Token()
{
    return _token;
}

const std::vector<TranslationLanguage>& AiSound::GetTranslationSrourceListData()
{
    return _srcTranslationLanguage;
}

const std::vector<TranslationLanguage>& AiSound::GetTranslationDestListData()
{
    return _destTranslationLanguage;
}

const std::vector<VoiceData>& AiSound::GetVoiceData()
{
    return _voiceData;
}

void AiSound::HttpCallbackDispatch(QNetworkReply* reply)
{
    HttpCallbackPacketRaw* packetRaw = dynamic_cast<HttpCallbackPacketRaw*>(reply->userData(Qt::UserRole));
    if (!packetRaw)
    {
        do
        {
            std::this_thread::yield();
            packetRaw = dynamic_cast<HttpCallbackPacketRaw*>(reply->userData(Qt::UserRole));
        }
        while (!packetRaw);
    }
    reply->setUserData(Qt::UserRole, nullptr);

    auto data = reply->readAll();

    switch (packetRaw->type)
    {
        case httpPasswordLogin:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString token = document["data"]["accessToken"].toString();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<PasswordLoginCallbackType>*>(packetRaw);
            UserLoginCallbackInternal(code, msg, token);
            packet->callback(code, msg, token);
        }
        break;

        case httpGetVerifyCode:
        {
            //QJsonParseError err_rpt;
            //auto document = QJsonDocument::fromJson(data, &err_rpt);
            //int code = document["code"].toInt();
            //QString msg = document["msg"].toString();
            //QString img = document["data"]["img"].toString();
            //QString uuid = document["data"]["uuid"].toString();

            //auto packet = dynamic_cast<HttpCallbackPacket<GetVerifyCodeCallbackType>*>(packetRaw);
            //packet->callback(code, msg, img, uuid);
        }
        break;

        case httpRegister:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<RegisterCallbackType>*>(packetRaw);
            packet->callback(code, msg);
        }
        break;

        case httpSendVerifyCode:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<SendVerifyCodeCallbackType>*>(packetRaw);
            packet->callback(code, msg);
        }
        break;

        case httpGetTranslationSource:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            std::vector<TranslationLanguage> vecLan;
            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["languaue"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                vecLan.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationSourceListCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecLan));
        }
        break;

        case httpGetTranslationDest:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            std::vector<TranslationLanguage> vecLan;
            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["languaue"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                vecLan.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationDestListCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecLan));
        }
        break;

        case httpGetVoiceSpeakerCallback:
        {
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            std::vector<VoiceData> vecVoiceData;
            auto data = document["data"].toArray();
            for (const auto& it : data)
            {
                VoiceData data;
                auto obj = it.toObject();
                data.id = obj["id"].toInt();
                data.voiceCode = obj["voiceCode"].toString();
                data.name = obj["name"].toString();
                data.language = obj["language"].toInt();
                data.gender = obj["gender"].toInt();
                vecVoiceData.push_back(data);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetVoiceSpeakerCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecVoiceData));
        }
        break;

        default:
        break;
    }
    reply->deleteLater();
    delete packetRaw;
}

//void AiSound::CompositorTest(const QString& token)
//{
//    _voiceCompositor.Connect(token);
//}

void AiSound::ChatBotTest(const QString& token)
{
    _chatBot.Connect(token);
}

void AiSound::FillTranslationFillList()
{
    GetTranslationSrourceList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                _srcTranslationLanguage = std::move(languageList);
            }
        });

    GetTranslationDestList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                _destTranslationLanguage = std::move(languageList);
            }
        });

    GetVoiceSpeaker([this](int code, const QString& msg, std::vector<VoiceData> vecVoiceData)
        {
            if (code == 200)
            {
                _voiceData = std::move(vecVoiceData);
            }
        });
}

void AiSound::UserLoginCallbackInternal(int code, const QString& msg, const QString& token)
{
    if (code == 200)
    {
        _token = token;
        FillTranslationFillList();
    }
}

void AiSound::NextMessage()
{
    if (!_ltMsg.empty())
    {
        auto msg =_ltMsg.front();
        _ltMsg.pop_front();
        _wTip->SetMessage(msg);
        _wTip->show();
    }
}

bool AiSound::AiFunctionRunning()
{
    return _translation.IsRunning() | _voiceCompositor.IsRunning() | _chatBot.IsRunning();
}

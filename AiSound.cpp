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

#include <Windows.h>
#include <thread>


AiSound AiSound::INSTANCE;

AiSound::AiSound()
{
    connect(&_networkAccess, &QNetworkAccessManager::finished, this, &AiSound::HttpCallbackDispatch);
}

AiSound::~AiSound()
{
}

void AiSound::Initialize()
{
    _wLoginFrame = new WLoginFrame{};
    _wTranslationSelect = new WTranslationSelect{};
    _wTranslationMain = new WTransaltionMain{};
    _wTip = new WTip{};
    connect(_wTip, &WTip::tipEnd, this, &AiSound::NextMessage);

    _translate.moveToThread(&_translateThread);
    _translate.Initialize();

    _voiceCompositor.Initialize();
}

AiSound& AiSound::GetInstance()
{
    return INSTANCE;
}

void AiSound::PasswordLogin(const QString& userName, const QString& password, PasswordLoginCallback callback)
{
    QNetworkRequest request;
    request.setUrl(QUrl{ "http://47.106.253.9:9101/user/loginByPwd" });
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
    request.setUrl(QUrl{ "http://47.106.253.9:9101/common/getImgVerifyCode" });
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
    request.setUrl(QUrl{ "http://47.106.253.9:9101/user/register"});
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
    request.setUrl(QUrl{ "http://47.106.253.9:9101/user/sendSmsVerifyCode" });
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
    request.setUrl(QUrl{ "http://47.106.253.9:9101/config/getSourceLanguaueList" });
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
    request.setUrl(QUrl{ "http://47.106.253.9:9101/config/getTargetLanguaueList" });
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    request.setRawHeader("access_token", _token.toUtf8());

    auto packet = new HttpCallbackPacket<GetTranslationSourceListCallbackType>();
    packet->type = httpGetTranslationDest;
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
    _wTranslationSelect->show();
}

void AiSound::ShowTranslationMainWindow()
{
    _wTranslationSelect->hide();
    _wTranslationMain->show();
}

void AiSound::TranslateConnect()
{
    _translateThread.start();
    _translate.Connect(_token);
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

Translate& AiSound::Translation()
{
    return _translate;
}

const QString& AiSound::Token()
{
    return _token;
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
            QJsonParseError err_rpt;
            auto document = QJsonDocument::fromJson(data, &err_rpt);
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            QString img = document["data"]["img"].toString();
            QString uuid = document["data"]["uuid"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<GetVerifyCodeCallbackType>*>(packetRaw);
            packet->callback(code, msg, img, uuid);
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
            packet->callback(code, msg, vecLan);
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
            packet->callback(code, msg, vecLan);
        }
        break;

        default:
        break;
    }

    delete packetRaw;
}

void AiSound::AudioTest(const QString& token)
{
    _translate.Connect(token);
}

void AiSound::CompositorTest(const QString& token)
{
    _voiceCompositor.Connect(token);
}

void AiSound::FillTranslationFillList()
{
    GetTranslationSrourceList([this](int code, const QString& msg, const std::vector<TranslationLanguage>& languageList)
        {
            if (code == 200) 
            {
                _wTranslationSelect->SetSrcLanguage(languageList);
            }
            else 
            {
                ShowTip(msg);
            }
        });

    GetTranslationDestList([this](int code, const QString& msg, const std::vector<TranslationLanguage>& languageList)
        {
            if (code == 200)
            {
                _wTranslationSelect->SetDestLanguage(languageList);
            }
            else
            {
                ShowTip(msg);
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

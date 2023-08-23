#include "Translation.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QTimerEvent>

#include <chrono>
#include <Windows.h>

Translation::Translation()
{
    qRegisterMetaType<TransType>();
    qRegisterMetaType<SystemLanguage>();
}

Translation::~Translation()
{
    Uninitialize();
}

void Translation::Initialize()
{
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &Translation::WebsocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &Translation::WebsocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &Translation::WebsocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &Translation::TranslateTextMessageReceived);
    this->moveToThread(&_workThread);

    _audioInput.Initialize();
    _audioMonitor.Initialize();
    _audioOutput.Initialize();
    QObject::connect(this, &Translation::connect, this, &Translation::ConnectInternal);
    QObject::connect(this, &Translation::disconnect, this, &Translation::DisconnectInternal);
    QObject::connect(&_audioInput, &AudioInput::audioInput, this, &Translation::ReceiveAudioInput);
    QObject::connect(&_audioMonitor, &AudioInput::audioInput, this, &Translation::ReceiveMonitorAudioInput);
}

void Translation::Uninitialize()
{
    if (_workThread.isRunning())
    {
        Disconnect();
    }

    _audioInput.Uninitialize();
}

void Translation::Connect(const QString& token, const QString& srcLan, const QString& destLan, TransType type, const QString& speaker, bool enableConvGuide, SystemLanguage language)
{
    _workThread.start();
    emit connect(token, srcLan, destLan, type, speaker, enableConvGuide, language);
}

void Translation::StartMic()
{
    _audioInput.StartMic();
}

void Translation::StopMic()
{
    _audioInput.EndMic();
}

void Translation::Disconnect()
{
    emit disconnect();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
}

void Translation::ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, TransType type, const QString& speaker, bool enableConvGuide, SystemLanguage language)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/st" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    _srcLan = srcLan;
    _destLan = destLan;
    _transType = type;
    _speaker = speaker;
    _isConvGuide = enableConvGuide;
    _language = language;

    auto str = url.toString();
    _webSocket.open(url);
}

void Translation::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;

    _audioInput.EndMic();
    SendFinish();
    _webSocket.close();
}

void Translation::EnableAudio(bool enable)
{
    if (enable)
    {
        _audioInput.StartMic();
        if (_isConvGuide)
        {
            _audioMonitor.StartMic();
            _audioOutput.StartSpeaker();
        }
    }
    else
    {
        _audioInput.EndMic();
        if (_isConvGuide)
        {
            _audioMonitor.EndMic();
            _audioOutput.EndSpeaker();
        }
    }
}

void Translation::SendParam()
{
    QJsonObject dataobj;
    dataobj.insert("type", "START");
    dataobj.insert("from", _srcLan);
    dataobj.insert("to", _destLan);
    switch (_transType)
    {
        case TRANSTYPE_NORMAL:
            dataobj.insert("transType", "2");
        break;

        case TRANSTYPE_HIGH_PRECISION:
            dataobj.insert("transType", "1");
        break;
    }

    dataobj.insert("ttsSpeaker", _speaker);
    dataobj.insert("isConvGuide", _isConvGuide ? 1 : 0);

    switch (_language)
    {
        case SYSTEM_LANGUAGE_CHS:
            dataobj.insert("systemLanguaue", "zh-cn");
        break;

        case SYSTEM_LANGUAGE_EN:
            dataobj.insert("systemLanguaue", "en_us");
        break;
    }

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void Translation::SendHearBeat()
{
    QJsonObject dataobj;
    dataobj.insert("type", "HEARTBEAT");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

bool Translation::Connected()
{
    return _connected;
}

bool Translation::IsRunning()
{
    return _workThread.isRunning();
}

void Translation::WebsocketConnected()
{
    SendHearBeat();
    SendParam();
    EnableAudio(true);
    _connected = true;
    _heartBeatTimer = startTimer(5000);
    emit connected();
}

void Translation::WebsocketDisconnected()
{
    EnableAudio(false);
    _connected = false;
    emit disconnected();
    _workThread.quit();
}

void Translation::WebsocketError(QAbstractSocket::SocketError error)
{
}

void Translation::TranslateTextMessageReceived(const QString& message)
{
    QJsonParseError err_rpt;
    auto document = QJsonDocument::fromJson(message.toUtf8(), &err_rpt);

    int code = document["code"].toInt();
    if (code == 0)
    {
        auto status = document["data"]["status"].toString();
        if (status == "TRAN")
        {
            auto obj = document["data"]["result"].toObject();
            auto dst = obj["dst"].toString();
            auto src = obj["src"].toString();
            auto type = obj["type"].toString();

            TranslationType iType{};
            if (type == "FIN")
            {
                iType = FIN;
            }
            else if (type == "MID")
            {
                iType = MID;
            }

            emit translationReceived(src, dst, iType);
        }

        if (status == "CG")
        {
            auto obj = document["data"]["result"].toObject();
            auto type = obj["type"].toString();
            auto message = obj["message"].toString();

            TranslationType iType{};
            if (type == "FIN")
            {
                iType = FIN;
            }
            else if (type == "MID")
            {
                iType = MID;
            }

            emit conversationGuideReceived(message, iType);
        }
    }
}

void Translation::SendFinish()
{
    QJsonObject dataobj;
    dataobj.insert("type", "FINISH");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void Translation::ReceiveAudioInput(QByteArray data)
{
    auto hex = data.toBase64();

    QString data64{ hex };

    QJsonObject dataobj;
    dataobj.insert("audio", data64);
    dataobj.insert("type", "RESULT");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void Translation::ReceiveMonitorAudioInput(QByteArray data)
{
    auto hex = data.toBase64();

    QString data64{ hex };

    QJsonObject dataobj;
    dataobj.insert("selfAudio", data64);
    dataobj.insert("type", "RESULT");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void Translation::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }
}

#include "Translation.h"
#include "AiDebug.h"
#include "AiSound.h"

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
    QObject::connect(this, &Translation::connect, this, &Translation::ConnectInternal);
    QObject::connect(this, &Translation::disconnect, this, &Translation::DisconnectInternal);
    QObject::connect(&_audioInput, &AudioLoop::audioInput, this, &Translation::ReceiveAudioInput);
    QObject::connect(&_audioInput, &AudioLoop::soundPlay, this, &Translation::SoundPlayInternal);
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

void Translation::Connect(const QString& token, const QString& srcLan, const QString& destLan, bool enableConvGuide, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& momitorDev, int type, SystemLanguage language)
{
    _workThread.start();
    emit connect(token, srcLan, destLan, enableConvGuide, micDev, momitorDev, type, language);
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

void Translation::ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, bool enableConvGuide, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& momitorDev, int type, SystemLanguage language)
{
    QUrl url{ "ws://api.sounda.ai/service/v1/st" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    _micDev = micDev;
    _momitorDev = momitorDev;
    _srcLan = srcLan;
    _destLan = destLan;
    _transType = type;
    _isConvGuide = enableConvGuide;
    _language = language;

    _receiveStart = false;

    auto str = url.toString();
    _webSocket.open(url);

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation connect:";
    debugText += str;
    AiDebug::Output(debugText);
#endif
}

void Translation::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;

    EnableAudio(false);
    SendFinish();
    _receiveStart = false;
    _webSocket.close();
}

void Translation::EnableAudio(bool enable)
{
    if (enable)
    {
        _audioInput.StartMic(_micDev);
        if (_isConvGuide)
        {
            _audioMonitor.StartMic(_momitorDev);
        }
    }
    else
    {
        _audioInput.EndMic();
        if (_isConvGuide)
        {
            _audioMonitor.EndMic();
        }
    }
}

void Translation::SendParam()
{
    QJsonObject dataobj;
    dataobj.insert("type", "START");
    dataobj.insert("from", _srcLan);
    dataobj.insert("to", _destLan);
    dataobj.insert("transType", _transType);
    dataobj.insert("guideFlag", _isConvGuide ? 1 : 0);
    auto token = AiSound::GetInstance().GetLanguageToken();
    dataobj.insert("systemLanguaue", token);

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation send:";
    debugText += byteArray;
    AiDebug::Output(debugText);
#endif

}

void Translation::SendHearBeat()
{
    QJsonObject dataobj;
    dataobj.insert("type", "HEARTBEAT");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation send:";
    debugText += byteArray;
    AiDebug::Output(debugText);
#endif
}

bool Translation::Connected()
{
    return _connected;
}

bool Translation::IsRunning()
{
    return _workThread.isRunning();
}

bool Translation::IsMicWorking()
{
    return _audioInput.IsRunning();
}

void Translation::WebsocketConnected()
{
    SendHearBeat();
    SendParam();
    EnableAudio(true);
    _connected = true;
    _heartBeatTimer = startTimer(5000);
    emit connected();
    emit stateChanged(TR_RUNING);
}

void Translation::WebsocketDisconnected()
{
    _connected = false;
    emit disconnected();
    emit stateChanged(TR_STOP);
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
        if (status == "START")
        {
            _receiveStart = true;
        }
        else if (status == "TRAN")
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
        else if (status == "CG")
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
    else
    {
        auto msg = document["msg"].toString();
        if (!msg.isEmpty())
        {
            emit showMessage(msg);
        }
    }

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation receive:";
    debugText += message;
    AiDebug::Output(debugText);
#endif
}

void Translation::SendFinish()
{
    QJsonObject dataobj;
    dataobj.insert("type", "FINISH");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation send:";
    debugText += byteArray;
    AiDebug::Output(debugText);
#endif
}

void Translation::SoundPlayInternal(bool play)
{
    emit soundPlay(play);
}

void Translation::ReceiveAudioInput(QByteArray data)
{
    if (!_receiveStart)
    {
        return;
    }

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
    if (!_receiveStart)
    {
        return;
    }

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

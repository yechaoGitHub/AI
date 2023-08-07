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
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &Translation::WebsocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &Translation::WebsocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &Translation::WebsocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &Translation::TranslateTextMessageReceived);
    this->moveToThread(&_workThread);
}

Translation::~Translation()
{
}

void Translation::Initialize()
{
    _audio.Initialize();
    QObject::connect(this, &Translation::connect, this, &Translation::ConnectInternal);
    QObject::connect(this, &Translation::disconnect, this, &Translation::DisconnectInternal);
    QObject::connect(&_audio, &AudioInput::audioInput, this, &Translation::ReceiveAudioInput);
}

void Translation::Connect(const QString& token, const QString& srcLan, const QString& destLan)
{
    _workThread.start();
    emit connect(token, srcLan, destLan);
}

void Translation::Disconnect()
{
    emit disconnect();
}

void Translation::ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/st" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    _srcLan = srcLan;
    _destLan = destLan;

    auto str = url.toString();
    _webSocket.open(url);
}

void Translation::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;

    _audio.EndMic();
    SendFinish();
    _webSocket.close();
}

void Translation::StartListen()
{
    _audio.StartMic();
}

void Translation::SendParam()
{
    QJsonObject dataobj;
    dataobj.insert("type", "START");
    dataobj.insert("from", _srcLan);
    dataobj.insert("to", _destLan);
    dataobj.insert("transType", "1");

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void Translation::SendHearBeat()
{
    _webSocket.sendTextMessage("{\"type\": \"HEARTBEAT\",}");
}

bool Translation::Connected()
{
    return _connected;
}

void Translation::WebsocketConnected()
{
    SendHearBeat();
    SendParam();
    StartListen();
    _connected = true;

    emit connected();

    _heartBeatTimer = startTimer(5000);
}

void Translation::WebsocketDisconnected()
{
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
    }
}

void Translation::SendFinish()
{
    _webSocket.sendTextMessage("{\"type\": \"FINISH\"");
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

void Translation::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }
}

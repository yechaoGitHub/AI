#include "Translate.h"
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


Translate::Translate()
{
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &Translate::WebsocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &Translate::WebsocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &Translate::WebsocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &Translate::TranslateTextMessageReceived);
    _audio.setParent(this);
}

Translate::~Translate()
{
}

void Translate::Initialize()
{
    QObject::connect(this, &Translate::connect, this, &Translate::ConnectInternal);
    QObject::connect(this, &Translate::disconnect, this, &Translate::DisconnectInternal);
    _audio.Initialize();
    QObject::connect(&_audio, &Audio::audioInput, this, &Translate::AudioInput);
}

void Translate::Connect(const QString& token)
{
    emit connect(token);
}

void Translate::Disconnect()
{
    emit disconnect();
}

void Translate::ConnectInternal(const QString& token)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/st" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    auto str = url.toString();
    _webSocket.open(url);
}

void Translate::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;

    _audio.EndListen();
    SendFinish();
    _webSocket.close();
}

void Translate::StartListen()
{
    _audio.StartListen();
}

void Translate::SendParam()
{
    _webSocket.sendTextMessage("{\"type\": \"START\", \"from\":\"zh_cn\",\"to\":\"en_us\",\"tts_speaker\":\"Lily\", \"transType\":1 }");
}

void Translate::SendHearBeat()
{
    _webSocket.sendTextMessage("{\"type\": \"HEARTBEAT\",}");
}

bool Translate::Connected()
{
    return _connected;
}

void Translate::WebsocketConnected()
{
    SendHearBeat();
    SendParam();
    StartListen();
    _connected = true;

    emit connected();

    _heartBeatTimer = startTimer(5000);
}

void Translate::WebsocketDisconnected()
{
    _connected = false;
    emit disconnected();
}

void Translate::WebsocketError(QAbstractSocket::SocketError error)
{
}

void Translate::TranslateTextMessageReceived(const QString& message)
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

void Translate::SendFinish()
{
    _webSocket.sendTextMessage("{\"type\": \"FINISH\"");
}

void Translate::AudioInput(QByteArray data)
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

void Translate::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }
}

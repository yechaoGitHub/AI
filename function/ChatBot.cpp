#include "ChatBot.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QTimerEvent>

ChatBot::ChatBot()
{
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &ChatBot::WebsocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &ChatBot::WebsocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &ChatBot::WebsocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &ChatBot::TranslateTextMessageReceived);
}

ChatBot::~ChatBot() 
{

}

void ChatBot::Initialize()
{
    QObject::connect(this, &ChatBot::connect, this, &ChatBot::ConnectInternal);
    QObject::connect(this, &ChatBot::disconnect, this, &ChatBot::DisconnectInternal);
    QObject::connect(this, &ChatBot::sendMessage, this, &ChatBot::SendMessageInternal);
}

void ChatBot::Connect(const QString& token)
{
    emit connect(token);
}

void ChatBot::Disconnect()
{
    emit disconnect();
}

bool ChatBot::Connected()
{
    return _connected;
}

void ChatBot::SendMessage(const QString& msg)
{
    emit sendMessage(msg);
}

void ChatBot::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
        SendMessage("请问1+1等于几？");
    }
}

void ChatBot::ConnectInternal(const QString& token)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/chat" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    auto str = url.toString();
    _webSocket.open(url);
}

void ChatBot::DisconnectInternal()
{
    _connected = false;
    emit disconnected();
}

void ChatBot::SendMessageInternal(const QString& msg)
{
    QJsonObject msgObj;
    msgObj.insert("type", "RESULT");
    msgObj.insert("message", msg);

    QJsonDocument document;
    document.setObject(msgObj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    _webSocket.sendTextMessage(byteArray);
}

void ChatBot::SendParam()
{
    _webSocket.sendTextMessage("{\"type\": \"START\", }");
}

void ChatBot::SendHearBeat()
{
    _webSocket.sendTextMessage("{\"type\": \"HEARTBEAT\",}");
}

void ChatBot::SendFinish()
{
    _webSocket.sendTextMessage("{\"type\": \"FINISH\"");
}

void ChatBot::WebsocketConnected()
{
    SendHearBeat();
    SendParam();
    _connected = true;
    emit connected();

    _heartBeatTimer = startTimer(5000);
}

void ChatBot::WebsocketDisconnected()
{
    _connected = false;
    emit disconnected();
}

void ChatBot::WebsocketError(QAbstractSocket::SocketError error)
{
}

void ChatBot::TranslateTextMessageReceived(const QString& message)
{
    QJsonParseError err_rpt;
    auto document = QJsonDocument::fromJson(message.toUtf8(), &err_rpt);

    int code = document["code"].toInt();
    if (code == 0)
    {
        auto status = document["data"]["status"].toString();
        if (status == "CHAT")
        {
            auto obj = document["data"]["result"].toObject();
            auto message = obj["message"].toString();
            auto type = obj["type"].toString();

            if (type == "FIN")
            {
                qDebug() << _receiveText;
                _receiveText.clear();
            }
            else if (type == "MID")
            {
                _receiveText.append(message);
            }
        }
    }
}

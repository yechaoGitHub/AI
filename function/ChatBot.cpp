#include "ChatBot.h"
#include "VoiceType.h"

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
}

ChatBot::~ChatBot()
{
    Uninitialize();
}

void ChatBot::Initialize()
{
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &ChatBot::WebsocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &ChatBot::WebsocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &ChatBot::WebsocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &ChatBot::TranslateTextMessageReceived);
    this->moveToThread(&_workThread);

    QObject::connect(this, &ChatBot::connect, this, &ChatBot::ConnectInternal);
    QObject::connect(this, &ChatBot::disconnect, this, &ChatBot::DisconnectInternal);
    QObject::connect(this, &ChatBot::sendMessage, this, &ChatBot::SendMessageInternal);
}

void ChatBot::Uninitialize()
{
    if (_workThread.isRunning())
    {
        Disconnect();
    }
}

void ChatBot::Connect(const QString& token, int id, const QString& conversationID)
{
    _workThread.start();
    emit connect(token, id, conversationID);
}

void ChatBot::Disconnect()
{
    emit disconnect();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
}

bool ChatBot::Connected()
{
    return _connected;
}

void ChatBot::SendMessage(const QString& msg)
{
    emit sendMessage(msg);
}

bool ChatBot::IsRunning()
{
    return _workThread.isRunning();
}

int ChatBot::SendMessageCount()
{
    return _counter;
}

int ChatBot::TemplateID()
{
    return _id;
}

QString ChatBot::ConversationID()
{
    return _conversationId;
}

void ChatBot::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }
}

void ChatBot::ConnectInternal(const QString& token, int id, const QString& conversationID)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/chat" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    _id = id;
    _conversationId = conversationID;

    auto str = url.toString();
    _webSocket.open(url);
}

void ChatBot::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;
    SendFinish();
    _webSocket.close();
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

    _counter++;
}

void ChatBot::SendParam()
{
    QJsonObject msgObj;
    msgObj.insert("type", "START");
    msgObj.insert("templateId", _id);
    if (_conversationId.size() > 0)
    {
        msgObj.insert("conversationId", _conversationId);
    }

    QJsonDocument document;
    document.setObject(msgObj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    _webSocket.sendTextMessage(byteArray);
}

void ChatBot::SendHearBeat()
{
    QJsonObject msgObj;
    msgObj.insert("type", "HEARTBEAT");

    QJsonDocument document;
    document.setObject(msgObj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    _webSocket.sendTextMessage(byteArray);
}

void ChatBot::SendFinish()
{
    QJsonObject msgObj;
    msgObj.insert("type", "FINISH");

    QJsonDocument document;
    document.setObject(msgObj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    _webSocket.sendTextMessage(byteArray);
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
    _workThread.quit();
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
            auto typeStr = obj["type"].toString();

            if (typeStr == "MID")
            {
                emit receiveText(MID, message);
            }
            else if (typeStr == "FIN")
            {
                emit receiveText(FIN, message);
            }
        }

        auto conversationId = document["data"]["result"]["conversationId"].toString();
        if (!conversationId.isEmpty())
        {
            _conversationId = conversationId;
        }
    }
}

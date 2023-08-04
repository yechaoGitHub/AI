#include "VoiceCompositor.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QTimerEvent>

VoiceCompositor::VoiceCompositor()
{
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &VoiceCompositor::SocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &VoiceCompositor::SocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &VoiceCompositor::SocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &VoiceCompositor::SocketTextMessageReceived);
    _audio.setParent(this);
}

VoiceCompositor::~VoiceCompositor()
{
}

void VoiceCompositor::Initialize()
{
    QObject::connect(this, &VoiceCompositor::connect, this, &VoiceCompositor::ConnectInternal);
    QObject::connect(this, &VoiceCompositor::disconnect, this, &VoiceCompositor::DisconnectInternal);
    _audio.Initialize();
    QObject::connect(&_audio, &Audio::audioInput, this, &VoiceCompositor::AudioInput);
}

void VoiceCompositor::Connect(const QString& token)
{
    emit connect(token);
}

void VoiceCompositor::Disconnect()
{
    emit disconnect();
}

void VoiceCompositor::SendParam()
{
    _webSocket.sendTextMessage("{\"type\": \"START\", \"from\":\"zh_cn\",\"to\":\"en_us\",\"tts_speaker\":\"xiaoyan\", \"isAutoSend\":true }");
}

void VoiceCompositor::SendHearBeat()
{
    _webSocket.sendTextMessage("{\"type\": \"HEARTBEAT\",}");
}

void VoiceCompositor::AudioStart()
{
    _audio.StartListen();
    _audio.StartPlay();
}

void VoiceCompositor::SendFinish()
{
    _webSocket.sendTextMessage("{\"type\": \"FINISH\"");
}

void VoiceCompositor::AudioInput(QByteArray data)
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

void VoiceCompositor::ConnectInternal(const QString& token)
{
    QUrl url{ "ws://47.106.253.9:9501/service/v1/tts" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);

    auto str = url.toString();
    _webSocket.open(url);
}

void VoiceCompositor::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;

    _audio.EndListen();
    SendFinish();
    _webSocket.close();
}

void VoiceCompositor::SocketConnected()
{
    SendHearBeat();
    SendParam();
    AudioStart();

    _heartBeatTimer = startTimer(5000);
    emit connected();

    _connected = true;
}

void VoiceCompositor::SocketError(QAbstractSocket::SocketError)
{
}

void VoiceCompositor::SocketDisconnected()
{
    _connected = false;
    emit disconnected();
}

void VoiceCompositor::SocketTextMessageReceived(const QString& message)
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

            qDebug() << src << "\n";
            qDebug() << dst << "\n";

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
        else if (status == "SGRE") 
        {
            auto obj = document["data"]["result"].toObject();
            auto type = obj["type"].toString();
            auto audio = obj["audio"].toString();
            if (type == "MID") 
            {
                _buffer.append(audio);
            }
            else if (type == "FIN")
            {
                _buffer.append(audio);
                auto audioData = QByteArray::fromBase64(_buffer.toLocal8Bit());

                qDebug() << "音频长度:" << audioData.size() << "\n";
                _audio.WriteOutputData(audioData);
                _buffer.clear();
            }
        }
    }
}

void VoiceCompositor::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }

    if (_connected) 
    {
        _webSocket.sendTextMessage("{\"type\": \"RESULT\", \"isSend\":true, \"message\": \"你好,给我声音 你好,给我声音\"}");
    }
}

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
    //_audioInput.setParent(this);
    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &VoiceCompositor::SocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &VoiceCompositor::SocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &VoiceCompositor::SocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &VoiceCompositor::SocketTextMessageReceived);
    this->moveToThread(&_workThread);
}

VoiceCompositor::~VoiceCompositor()
{
    Uninitialize();
}

void VoiceCompositor::Initialize()
{
    _audioInput.Initialize();
    _audioOutput.Initialize();

    QObject::connect(this, &VoiceCompositor::connect, this, &VoiceCompositor::ConnectInternal);
    QObject::connect(this, &VoiceCompositor::disconnect, this, &VoiceCompositor::DisconnectInternal);
    QObject::connect(&_audioInput, &AudioInput::audioInput, this, &VoiceCompositor::ReceiveAudioInput);
}

void VoiceCompositor::Uninitialize()
{
    if (_workThread.isRunning())
    {
        Disconnect();
    }

    while (_workThread.isRunning())
    {
        std::this_thread::yield();
    }
}

void VoiceCompositor::Connect(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender)
{
    _workThread.start();
    emit connect(token, srcLan, destLan, speaker, autoSender);
}

void VoiceCompositor::Disconnect()
{
    emit disconnect();
}

void VoiceCompositor::SendParam()
{
    QJsonObject dataobj;
    dataobj.insert("type", "START");
    dataobj.insert("from", _srcLan);
    dataobj.insert("to", _destLan);
    dataobj.insert("tts_speaker", _speaker);
    dataobj.insert("isAutoSend", _autoSender);

    QJsonDocument document;
    document.setObject(dataobj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _webSocket.sendTextMessage(byteArray);
}

void VoiceCompositor::SendHearBeat()
{
    _webSocket.sendTextMessage("{\"type\": \"HEARTBEAT\",}");
}

void VoiceCompositor::AudioStart(bool enable)
{
    if (enable)
    {
        _audioInput.StartMic();
        _audioOutput.StartSpeaker();
    }
    else
    {
        _audioInput.EndMic();
        _audioOutput.EndSpeaker();
    }
}

void VoiceCompositor::SendFinish()
{
    _webSocket.sendTextMessage("{\"type\": \"FINISH\"");
}

void VoiceCompositor::ReceiveAudioInput(QByteArray data)
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

void VoiceCompositor::ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender)
{
    _srcLan = srcLan;
    _destLan = destLan;
    _speaker = speaker;
    _autoSender = autoSender;

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
    SendFinish();
    _webSocket.close();
}

void VoiceCompositor::SocketConnected()
{
    SendHearBeat();
    SendParam();
    AudioStart(true);

    _heartBeatTimer = startTimer(5000);
    _connected = true;
    emit connected();
}

void VoiceCompositor::SocketError(QAbstractSocket::SocketError)
{
}

void VoiceCompositor::SocketDisconnected()
{
    AudioStart(false);
    _connected = false;
    emit disconnected();
    _workThread.quit();
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

                _audioOutput.WriteOutputData(audioData);
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
}

#include "VoiceCompositor.h"
#include "AiSound.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QTimerEvent>

#include "AiDebug.h"

VoiceCompositor::VoiceCompositor()
{
}

VoiceCompositor::~VoiceCompositor()
{
    Uninitialize();
}

void VoiceCompositor::Initialize()
{
    _transformMp3.Initialize(16000, AV_SAMPLE_FMT_S16P, 2);

    _webSocket.setParent(this);
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &VoiceCompositor::SocketConnected);
    QObject::connect(&_webSocket, &QWebSocket::disconnected, this, &VoiceCompositor::SocketDisconnected);
    QObject::connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this, &VoiceCompositor::SocketError);
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &VoiceCompositor::SocketTextMessageReceived);
    this->moveToThread(&_workThread);

    _audioInput.Initialize();
    _audioOutput.Initialize();

    QObject::connect(this, &VoiceCompositor::connect, this, &VoiceCompositor::ConnectInternal);
    QObject::connect(this, &VoiceCompositor::disconnect, this, &VoiceCompositor::DisconnectInternal);
    QObject::connect(this, &VoiceCompositor::sendMessage, this, &VoiceCompositor::SendMessageInternal);
    QObject::connect(&_audioInput, &AudioInput::audioInput, this, &VoiceCompositor::ReceiveAudioInput);
    QObject::connect(&_audioInput, &AudioInput::soundPlay, this, &VoiceCompositor::SoundPlayInternal);
}

void VoiceCompositor::Uninitialize()
{
    if (_workThread.isRunning())
    {
        Disconnect();
    }

    _audioInput.Uninitialize();
    _audioOutput.Uninitialize();
    _transformMp3.Clear();
}

void VoiceCompositor::Connect(const QString& token, const QString& srcLan, int speaker, bool autoSender, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& speakerDev)
{
    _workThread.start();
    emit connect(token, srcLan, speaker, autoSender, micDev, speakerDev);
}

void VoiceCompositor::SendChatMessage(const QString& msg)
{
    emit sendMessage(msg);
}

bool VoiceCompositor::SaveMp3(const QString& savePath)
{
    return _transformMp3.TransformToMp3(savePath, _voiceBuffer);
}

void VoiceCompositor::Disconnect()
{
    emit disconnect();
    if (!_workThread.wait(1000))
    {
        _workThread.quit();
    }
}

bool VoiceCompositor::IsRunning()
{
    return _workThread.isRunning();
}

bool VoiceCompositor::IsMicWorking()
{
    return _audioInput.IsRunning();
}

void VoiceCompositor::StartMic()
{
    _audioInput.StartMic();
}

void VoiceCompositor::StopMic()
{
    _audioInput.EndMic();
}

void VoiceCompositor::SendParam()
{
    QJsonObject dataobj;
    dataobj.insert("type", "START");
    dataobj.insert("from", _srcLan);
    dataobj.insert("ttsSpeaker", _speaker);
    dataobj.insert("isAutoSend", _autoSender);
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

void VoiceCompositor::SendHearBeat()
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

void VoiceCompositor::AudioStart(bool enable)
{
    if (enable)
    {
        _audioInput.StartMic(_micDev);
        _audioOutput.StartSpeaker(_speakerDev);
    }
    else
    {
        _audioInput.EndMic();
        _audioOutput.EndSpeaker();
    }
}

void VoiceCompositor::SendFinish()
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

void VoiceCompositor::ConnectInternal(const QString& token, const QString& srcLan, int speaker, bool autoSender, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& speakerDev)
{
    _micDev = micDev;
    _speakerDev = speakerDev;
    _srcLan = srcLan;
    _speaker = speaker;
    _autoSender = autoSender;

    QUrl url{ "ws://47.106.253.9:9501/service/v1/tts" };
    QUrlQuery quurl;
    quurl.addQueryItem("access_token", token);
    url.setQuery(quurl);
    auto str = url.toString();
    _webSocket.open(url);

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation connect:";
    debugText += str;
    AiDebug::Output(debugText);
#endif

}

void VoiceCompositor::DisconnectInternal()
{
    killTimer(_heartBeatTimer);
    _heartBeatTimer = 0;
    SendFinish();
    _webSocket.close();
}

void VoiceCompositor::SendMessageInternal(const QString& msg)
{
    if (!_connected)
    {
        return;
    }

    QJsonObject dataobj;
    dataobj.insert("type", "RESULT");
    dataobj.insert("isSend", true);
    dataobj.insert("message", msg);

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

void VoiceCompositor::SoundPlayInternal(bool play)
{
    emit soundPlay(play);
}

void VoiceCompositor::SocketConnected()
{
    SendHearBeat();
    SendParam();
    AudioStart(true);
    _heartBeatTimer = startTimer(5000);
    _connected = true;
    emit connected();
    emit stateChanged(VC_RUNING);
}

void VoiceCompositor::SocketError(QAbstractSocket::SocketError)
{
}

void VoiceCompositor::SocketDisconnected()
{
    AudioStart(false);
    _connected = false;
    emit disconnected();
    emit stateChanged(VC_STOP);
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

            auto audioData = QByteArray::fromBase64(audio.toLocal8Bit());
            _buffer.append(std::move(audioData));
            if (type == "FIN")
            {
                _audioOutput.WriteOutputData(_buffer);
                _voiceBuffer = std::move(_buffer);
                _buffer.clear();

                emit soundRecive();
            }
        }
    }

#if _AI_DEBUG
    QString debugText;
    debugText = "Translation receive:";
    debugText += message;
    AiDebug::Output(debugText);
#endif
}

void VoiceCompositor::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == _heartBeatTimer)
    {
        SendHearBeat();
    }
}

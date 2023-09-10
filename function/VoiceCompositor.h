#pragma once
#include "VoiceType.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "QPcmToMp3.h"

#include <QWebsocket>
#include <QThread>

class VoiceCompositor : public QObject
{
    Q_OBJECT
public:
    VoiceCompositor();
    ~VoiceCompositor();

    void Initialize();
    void Uninitialize();
    void Connect(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender, const QAudioDeviceInfo& micDev = {}, const QAudioDeviceInfo& speakerDev = {});
    void SendMessage(const QString& msg);
    bool SaveMp3(const QString& savePath);
    void Disconnect();
    bool IsRunning();

Q_SIGNALS:
    void connected();
    void disconnected();
    void translationReceived(const QString& src, const QString& dst, int type);

private:
Q_SIGNALS:
    void connect(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& speakerDev);
    void disconnect();
    void receiveAudio(const QByteArray& data);
    void sendMessage(const QString& msg);

private:
    void ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& speakerDev);
    void DisconnectInternal();
    void SendMessageInternal(const QString& msg);

    void SendParam();
    void SendHearBeat();
    void AudioStart(bool enable);
    void SendFinish();

    void ReceiveAudioInput(QByteArray data);

    void SocketConnected();
    void SocketDisconnected();
    void SocketError(QAbstractSocket::SocketError);
    void SocketTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    QAudioDeviceInfo        _micDev;
    QAudioDeviceInfo        _speakerDev;
    AudioInput              _audioInput;
    AudioOutput             _audioOutput;
    QPcmToMp3               _transformMp3;
    QWebSocket              _webSocket;
    QThread                 _workThread;
    QString                 _srcLan;
    QString                 _destLan;
    QString                 _speaker;
    bool                    _autoSender = false;

    QByteArray              _voiceBuffer;
    QByteArray              _buffer;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


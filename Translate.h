#pragma once

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QWebsocket>
#include "Audio.h"
#include "VoiceType.h"


#define AUDIO_OUTPUT 0

class Translate : public QObject
{
    Q_OBJECT
public:
    Translate();
    ~Translate();

    void Initialize();

    void TranslateConnect(const QString& token);
    void TranslateDisconnect();
    bool Connected();

Q_SIGNALS:
    void connected();
    void disconnected();
    void translationReceived(const QString& src, const QString& dst, int type);
    void translateConnect(const QString& token);
    void translateDisconnect();

private:
    void TranslateConnectInternal(const QString& token);
    void TranslateDisconnectInternal();

    void StartListen();
    void SendParam();
    void SendHearBeat();
    void SendFinish();

    void AudioInput(QByteArray data);

    void TranslateConnected();
    void TranslateDisconnected();
    void TranslateError(QAbstractSocket::SocketError error);
    void TranslateTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    Audio                   _audio;
    QWebSocket              _translateSocket;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;

#if AUDIO_OUTPUT
    QAudioOutput*           _audioOutput = nullptr;
    QIODevice*              _out = nullptr;
#endif
};


#pragma once

#include "Audio.h"
#include "VoiceType.h"
#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QWebsocket>
#include <QThread>

class Translation : public QObject
{
    Q_OBJECT
public:
    Translation();
    ~Translation();

    void Initialize();
    void Connect(const QString& token, const QString& srcLan, const QString& destLan);
    void Disconnect();
    bool Connected();

Q_SIGNALS:
    void connect(const QString& token, const QString& srcLan, const QString& destLan);
    void disconnect();

    void connected();
    void disconnected();
    void translationReceived(const QString& src, const QString& dst, int type);

private:
    void ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan);
    void DisconnectInternal();

    void StartListen();
    void SendParam();
    void SendHearBeat();
    void SendFinish();

    void AudioOutput(QByteArray data);

    void WebsocketConnected();
    void WebsocketDisconnected();
    void WebsocketError(QAbstractSocket::SocketError error);
    void TranslateTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    Audio                   _audio;
    QWebSocket              _webSocket;
    QString                 _srcLan;
    QString                 _destLan;
    QThread                 _workThread;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


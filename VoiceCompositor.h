#pragma once
#include "Audio.h"
#include "VoiceType.h"

#include <QWebsocket>

class VoiceCompositor : public QObject
{
    Q_OBJECT
public:
    VoiceCompositor();
    ~VoiceCompositor();

    void Initialize();
    void Connect(const QString& token);
    void Disconnect();


Q_SIGNALS:
    void connected();
    void disconnected();

    void translationReceived(const QString& src, const QString& dst, int type);
    void connect(const QString& token);
    void disconnect();

    void receiveAudio(const QByteArray& data);

private:
    void ConnectInternal(const QString& token);
    void DisconnectInternal();

    void SendParam();
    void SendHearBeat();
    void AudioStart();
    void SendFinish();

    void AudioInput(QByteArray data);

    void SocketConnected();
    void SocketDisconnected();
    void SocketError(QAbstractSocket::SocketError);
    void SocketTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    Audio                   _audio;
    QWebSocket              _webSocket;
    QString                 _buffer;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


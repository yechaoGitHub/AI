#pragma once
#include "Audio.h"
#include "VoiceType.h"

#include <QWebsocket>
#include <QThread>

class VoiceCompositor : public QObject
{
    Q_OBJECT
public:
    VoiceCompositor();
    ~VoiceCompositor();

    void Initialize();
    void Connect(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender);
    void Disconnect();

Q_SIGNALS:
    void connected();
    void disconnected();

    void translationReceived(const QString& src, const QString& dst, int type);
    void connect(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender);
    void disconnect();

    void receiveAudio(const QByteArray& data);

private:
    void ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, const QString& speaker, bool autoSender);
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
    QThread                 _workThread;
    QString                 _srcLan;
    QString                 _destLan;
    QString                 _speaker;
    bool                    _autoSender = false;

    QString                 _buffer;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


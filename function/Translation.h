#pragma once

#include "VoiceType.h"
#include "AudioInput.h"
#include "AudioOutput.h"

#include <QObject>
#include <QWebsocket>
#include <QThread>

enum TransType { TRANSTYPE_NORMAL, TRANSTYPE_HIGH_PRECISION };
Q_DECLARE_METATYPE(TransType);
enum SystemLanguage{ SYSTEM_LANGUAGE_CHS, SYSTEM_LANGUAGE_EN };
Q_DECLARE_METATYPE(SystemLanguage);

class Translation : public QObject
{
    Q_OBJECT
public:
    Translation();
    ~Translation();

    void Initialize();
    void Uninitialize();
    void Connect(const QString& token, const QString& srcLan, const QString& destLan, TransType type = TRANSTYPE_NORMAL, const QString& speaker = "", bool enableConvGuide = false, SystemLanguage language = SYSTEM_LANGUAGE_CHS);

    void StartMic();
    void StopMic();

    void Disconnect();
    bool Connected();
    bool IsRunning();

Q_SIGNALS:
    void connect(const QString& token, const QString& srcLan, const QString& destLan, TransType type, const QString& speaker, bool enableConvGuide, SystemLanguage language);
    void disconnect();

    void connected();
    void disconnected();
    void translationReceived(const QString& src, const QString& dst, int type);
    void conversationGuideReceived(const QString& message, int type);

private:
    void ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, TransType type, const QString& speaker, bool enableConvGuide, SystemLanguage language);
    void DisconnectInternal();

    void EnableAudio(bool enable);
    void SendParam();
    void SendHearBeat();
    void SendFinish();

    void ReceiveAudioInput(QByteArray data);
    void ReceiveMonitorAudioInput(QByteArray data);

    void WebsocketConnected();
    void WebsocketDisconnected();
    void WebsocketError(QAbstractSocket::SocketError error);
    void TranslateTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    AudioInput              _audioInput;
    AudioInput              _audioMonitor;
    AudioOutput             _audioOutput;
    QWebSocket              _webSocket;
    QThread                 _workThread;
    QString                 _srcLan;
    QString                 _destLan;
    TransType               _transType;
    QString                 _speaker;
    bool                    _isConvGuide;
    SystemLanguage          _language;

    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


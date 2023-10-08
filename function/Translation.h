#pragma once

#include "VoiceType.h"
#include "AudioInput.h"
#include "AudioOutput.h"

#include <QObject>
#include <QWebsocket>
#include <QThread>

enum TransType { TRANSTYPE_NORMAL = 2, TRANSTYPE_HIGH_PRECISION = 1 };
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
    void Connect(const QString& token, const QString& srcLan, const QString& destLan, bool enableConvGuide = false, const QAudioDeviceInfo& micDev = {}, const QAudioDeviceInfo& momitorDev = {}, int type = TRANSTYPE_NORMAL, SystemLanguage language = SYSTEM_LANGUAGE_CHS);

    void StartMic();
    void StopMic();

    void Disconnect();
    bool Connected();
    bool IsRunning();
    bool IsMicWorking();

Q_SIGNALS:
    void connect(const QString& token, const QString& srcLan, const QString& destLan, bool enableConvGuide, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& momitorDev, int type, SystemLanguage language);
    void disconnect();
    void soundPlay(bool play);
    void stateChanged();

    void connected();
    void disconnected();
    void translationReceived(const QString& src, const QString& dst, int type);
    void conversationGuideReceived(const QString& message, int type);

private:
    void ConnectInternal(const QString& token, const QString& srcLan, const QString& destLan, bool enableConvGuide, const QAudioDeviceInfo& micDev, const QAudioDeviceInfo& momitorDev, int type, SystemLanguage language);
    void DisconnectInternal();

    void EnableAudio(bool enable);
    void SendParam();
    void SendHearBeat();
    void SendFinish();

    void SoundPlayInternal(bool play);
    void ReceiveAudioInput(QByteArray data);
    void ReceiveMonitorAudioInput(QByteArray data);

    void WebsocketConnected();
    void WebsocketDisconnected();
    void WebsocketError(QAbstractSocket::SocketError error);
    void TranslateTextMessageReceived(const QString& message);
    void timerEvent(QTimerEvent* ev) override;

    QAudioDeviceInfo        _micDev;
    QAudioDeviceInfo        _momitorDev;
    AudioInput              _audioInput;
    AudioInput              _audioMonitor;
    AudioOutput             _audioOutput;
    QWebSocket              _webSocket;
    QThread                 _workThread;
    QString                 _srcLan;
    QString                 _destLan;
    int                     _transType;
    QString                 _speaker;
    bool                    _isConvGuide;
    SystemLanguage          _language;

    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
};


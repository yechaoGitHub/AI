#pragma once
#include <QObject>
#include <QWebSocket>
#include <QThread>

class ChatBot : public QObject
{
    Q_OBJECT
public:
    ChatBot();
    ~ChatBot();

    void Initialize();
    void Uninitialize();
    void Connect(const QString& token);
    void Disconnect();
    bool Connected();
    void SendMessage(const QString& msg);
    bool IsRunning();

Q_SIGNALS:
    void connect(const QString& token);
    void disconnect();
    void receiveText(QString text);

    void connected();
    void disconnected();
    void sendMessage(const QString& msg);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void ConnectInternal(const QString& token);
    void DisconnectInternal();
    void SendMessageInternal(const QString& msg);

    void SendParam();
    void SendHearBeat();
    void SendFinish();

    void WebsocketConnected();
    void WebsocketDisconnected();
    void WebsocketError(QAbstractSocket::SocketError error);
    void TranslateTextMessageReceived(const QString& message);

    QWebSocket              _webSocket;
    QThread                 _workThread;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
    QString                 _receiveText;
};

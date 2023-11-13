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
    void Connect(const QString& token, int id, const QString& conversationID);
    void Disconnect();
    bool Connected();
    void SendChatMessage(const QString& msg);
    bool IsRunning();
    int SendMessageCount();

    int TemplateID();
    QString ConversationID();

Q_SIGNALS:
    void connect(const QString& token, int id, const QString& conversationID);
    void disconnect();
    void receiveText(int type, QString text);
    void newConnect();
    void showMessage(QString msg);

    void connected();
    void disconnected();
    void sendMessage(const QString& msg);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void ConnectInternal(const QString& token, int id, const QString& conversationID);
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
    int                     _id = 1;
    int                     _counter = 0;
    bool                    _connected = false;
    int                     _heartBeatTimer = 0;
    bool                    _receiveStart = false;
    QString                 _receiveText;
    QString                 _conversationId;
};

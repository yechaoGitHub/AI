#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_loginMainFrame.h"
#include "WLogin.h"
#include "WRegister.h"
#include <QAudioOutput>
#include <QWebSocket>
#include <QNetworkAccessManager>

class WLoginFrame : public QWidget
{
    Q_OBJECT

public:
    WLoginFrame(QWidget *parent = nullptr);
    ~WLoginFrame();

private:
    void RegisterClicked();
    void CommitRegisterBtnClicked();
    void LoginBtnClicked();

    void handleStateChanged(QAudio::State newState);

    void Connected();
    void Disconnected();
    void TextMessageReceived(const QString& message);
    void StateChanged(QAbstractSocket::SocketState state);

    void receiveGaodePostReply(QNetworkReply* reply);

    Ui::loginMainFrame      ui;
    WLogin*                 _login;
    WRegister*              _signUp;

    QPoint                  _clickPos;
    QWebSocket*             _client;
    QNetworkAccessManager*  _accessManager;
};

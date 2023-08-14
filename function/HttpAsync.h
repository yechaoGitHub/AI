#pragma once

#include <QNetworkReply>
#include <QJsonObject>
#include <QThread>

class HttpAsync : public QObject
{
    Q_OBJECT
public:
    enum class HttpResult{ SUCCESS, FAIL, TIMEOUT };

public:
    HttpAsync();
    ~HttpAsync();

    void Initialize();

    void Post(const QString& url, const QJsonObject& param = {}, const QMap<QString, QString>& headers = {}, QVariant userParam = 0);

private:
    Q_SIGNALS:
        void post(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam);
        void httpRespond(HttpResult result, int code, const QString& content, const QVariant& userParam);

private:
    void PostInternal(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam);

    QThread                 _workThread;
    QNetworkAccessManager   _manager;
};

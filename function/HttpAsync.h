#pragma once

#include <QNetworkReply>
#include <QJsonObject>
#include <QThread>

class HttpAsync : public QObject
{
    Q_OBJECT
public:
    enum class HttpResult{ SUCCESS, FAIL, TIMEOUT, ALEADY_EXIST };

public:
    HttpAsync();
    ~HttpAsync();

    void Initialize();
    void Uninitialize();

    void Post(const QString& url, const QJsonObject& param = {}, const QMap<QString, QString>& headers = {}, QVariant userParam = 0);
    void Download(const QString& url, const QString& savePath);

Q_SIGNALS:
    void httpRespond(HttpResult result, int code, const QByteArray& content, const QVariant& userParam);
    void downloadRespond(HttpResult result, const QString& url, const QString& savePath);

    void post(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam);
    void download(const QString& url, const QString& savePath);

private:
    void PostInternal(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam);
    void DownloadInternal(const QString& url, const QString& savePath);

    QThread                 _workThread;
    QNetworkAccessManager   _manager;
};

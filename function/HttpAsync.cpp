#include "HttpAsync.h"

#include "base/HttpClient.h"

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QFileInfo>

#include "AiDebug.h"

HttpAsync::HttpAsync()
{
}

HttpAsync::~HttpAsync()
{
    Uninitialize();
}

void HttpAsync::Initialize()
{
    qRegisterMetaType<QMap<QString, QString>>("StringMap");
    qRegisterMetaType<HttpResult>("HttpResult");

    _manager.setParent(this);
    connect(this, &HttpAsync::post, this, &HttpAsync::PostInternal);
    connect(this, &HttpAsync::download, this, &HttpAsync::DownloadInternal);

    this->moveToThread(&_workThread);
    _workThread.start();
}

void HttpAsync::Uninitialize()
{
    _workThread.quit();
}

void HttpAsync::Post(const QString& url, const QJsonObject& param, const QMap<QString, QString>& headers, QVariant userParam)
{
    emit post(url, param, headers, userParam);
}

void HttpAsync::Download(const QString& url, const QString& savePath)
{
    emit download(url, savePath);
}

void HttpAsync::PostInternal(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam)
{
    HttpClient* client = new HttpClient{ url };
    client->manager(&_manager);

    //client->header("Content-Type", "application/x-www-form-urlencoded");
    if (!headers.isEmpty())
    {
        client->headers(headers);
    }

#ifdef _AI_DEBUG
    QString debugText;
    debugText = "http call:";
    debugText += url;
#endif

    //if (!param.isEmpty())
    {
        QJsonDocument document;
        document.setObject(param);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        client->json(byteArray);

#ifdef _DEBUG
        QString debugText;
        debugText += " param:";
        debugText += byteArray;
#endif
    }

#ifdef _AI_DEBUG
    AiDebug::Output(debugText);
#endif

    client->success([this, url, client, userParam](const QString& response) {
        emit httpRespond(HttpResult::SUCCESS, -1, response, userParam);
        delete client;
#ifdef _AI_DEBUG
        QString debugText;
        debugText = "http responed:";
        debugText += url;
        debugText += " content: ";
        debugText += response;
        AiDebug::Output(debugText);
#endif
        });
    client->timeout([this, url, client, userParam]() {
        emit httpRespond(HttpResult::TIMEOUT, -1, "", userParam);
        delete client;
#ifdef _AI_DEBUG
        QString debugText;
        debugText = "http responed:";
        debugText += url;
        debugText += " time out";
        AiDebug::Output(debugText);
#endif
        });
    client->fail([this, url, client, userParam](const QString& response, int code) {
        emit httpRespond(HttpResult::FAIL, -1, response, userParam);
        delete client;
#ifdef _AI_DEBUG
        QString debugText;
        debugText = "http responed:";
        debugText += url;
        debugText += " failed";
        AiDebug::Output(debugText);
#endif
        });
    client->timeout(10).post();
}

void HttpAsync::DownloadInternal(const QString& url, const QString& savePath)
{
    QFileInfo fileInfo(savePath);
    if (fileInfo.isFile())
    {
        emit downloadRespond(HttpResult::ALEADY_EXIST, url, savePath);
        return;
    }

    HttpClient* client = new HttpClient{ url };
    client->manager(&_manager);

    client->success([this, client, url, savePath](const QString& response) {
        emit downloadRespond(HttpResult::SUCCESS, url, savePath);
        delete client;
        });
    client->timeout([this, client, url, savePath]() {
        emit downloadRespond(HttpResult::TIMEOUT, url, savePath);
        delete client;
        });
    client->fail([this, client, url, savePath](const QString& response, int code) {
        emit downloadRespond(HttpResult::FAIL, url, savePath);
        delete client;
        });

    client->download(savePath);
}

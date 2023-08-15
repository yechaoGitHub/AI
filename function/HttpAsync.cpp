#include "HttpAsync.h"

#include "base/HttpClient.h"

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>

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

void HttpAsync::PostInternal(const QString& url, const QJsonObject& param, QMap<QString, QString> headers, QVariant userParam)
{
    HttpClient* client = new HttpClient{ url };
    client->manager(&_manager);

    //client->header("Content-Type", "application/x-www-form-urlencoded");
    if (!headers.isEmpty())
    {
        client->headers(headers);
    }

    if (!param.isEmpty())
    {
        QJsonDocument document;
        document.setObject(param);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        client->json(byteArray);
    }

    client->success([this, client, userParam](const QString& response) {
        emit httpRespond(HttpResult::SUCCESS, -1, response, userParam);
        delete client;
        });
    client->timeout([this, client, userParam]() {
        emit httpRespond(HttpResult::TIMEOUT, -1, "", userParam);
        delete client;
        });
    client->fail([this, client, userParam](const QString& response, int code) {
        emit httpRespond(HttpResult::FAIL, -1, response, userParam);
        delete client;
        });
    client->timeout(10).post();

}

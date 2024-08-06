#include "quotesapi.h"
#include "qeventloop.h"
#include "qurl.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>


QuotesLogApi::QuotesLogApi(std::shared_ptr<Connection> conn, std::shared_ptr<User> user):
    _conn(conn), _user(user)
{
    _source = "http://" + _conn->server() + ":" + QString::number(_conn->port());
    _manager = new QNetworkAccessManager(this);
}

QuotesLogApi::~QuotesLogApi()
{
    delete _manager;
}

int QuotesLogApi::auth()
{
    QString mapping = "/auth/sign-in";
    QNetworkRequest authRequest;

    authRequest.setUrl(QUrl(_source + mapping));

    authRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json
    {
        {"username", _user->username()},
        {"password", _user->passwd()}
    };
    QJsonDocument doc(json);
    QByteArray authentication = doc.toJson();

    QNetworkReply* reply = _manager->post(authRequest, authentication);

    QEventLoop loop;
    QAbstractSocket::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

        if (!doc.isObject()) return false;

        QJsonObject jsonReply = doc.object();

        QJsonValue token = jsonReply["token"];
        QJsonValue lifetime = jsonReply["lifetime"];
        _accessToken = token.toString();
        qDebug() << "authed";
        return lifetime.toInt();
    }
    else
    {
        qDebug() << "Authentication false: error in reply" << reply->errorString() << _user->username() << _user->passwd();
        return 0;
    }
}

QNetworkReply *QuotesLogApi::addUser(User* user)
{
    QString mapping = "/auth/sign-up";
    QNetworkRequest registerRequest;

    registerRequest.setUrl(QUrl(_source + mapping));

    registerRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json
        {
            {"username", user->username()},
            {"password", user->passwd()}
        };
    QJsonDocument doc(json);
    QByteArray registration = doc.toJson();

    qDebug() << json << registration << "\n" << _source + mapping;

    QNetworkReply* reply = _manager->post(registerRequest, registration);
    return reply;
}

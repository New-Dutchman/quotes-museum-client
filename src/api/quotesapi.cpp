#include "quotesapi.h"
#include "qeventloop.h"
#include "qurl.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>


QuotesApi::QuotesApi(std::shared_ptr<Connection> conn, std::shared_ptr<User> user):
    _conn(conn), _user(user)
{
    _source = "http://" + _conn->server() + ":" + QString::number(_conn->port());
    _manager = new QNetworkAccessManager(this);
}

QuotesApi::~QuotesApi()
{
    delete _manager;
}

int QuotesApi::auth()
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

QNetworkReply *QuotesApi::addUser(User* user)
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

QNetworkReply *QuotesApi::addQuote(SingleQuoteModel *data)
{
    QString mapping = "/inside/add-quote";
    QNetworkRequest quoteRequest;

    QString bearerToken = "Bearer " + _accessToken;
    quoteRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    quoteRequest.setUrl(QUrl(_source + mapping));

    quoteRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc = data->serializeToQJsonDocument();

    QByteArray jsonQuote = doc.toJson();
    qDebug() << doc;
    qDebug() << jsonQuote;

    QNetworkReply* reply = _manager->post(quoteRequest, jsonQuote);

    return reply;
}

QNetworkReply *QuotesApi::addOwner(std::pair<QString, QString> owner)
{
    QString mapping = "/inside/add-owner";

    QUrl url(_source + mapping);
    QNetworkRequest ownerRequest;

    ownerRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString bearerToken = "Bearer " + _accessToken;
    ownerRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    ownerRequest.setUrl(url);
    QJsonObject json
        {
            {"owner", owner.first},
            {"description", owner.second}
        };

    QJsonDocument doc(json);
    QByteArray jsonOwner = doc.toJson();

    QNetworkReply* reply = _manager->post(ownerRequest, jsonOwner);
    return reply;
}

QNetworkReply *QuotesApi::updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original)
{

    assert((changed->id() == original->id()) && (changed->cites() == original->cites()));
    bool attrsChanged = changed->attrs() != original->attrs();
    bool featuresChanged = changed->features() != original->features();
    bool quoteChanged = (changed->is_obscene() != original->is_obscene()) ||
                        (changed->quote() != original->quote()) ||
                        (changed->owner() != original->owner());

    qDebug() << changed->is_obscene() << original->is_obscene() <<
                changed->quote() << original->quote() <<
                changed->owner() << original->owner() <<
                changed->id() << original->id() <<
                changed->cites() << original->cites();

    qDebug() << (changed->is_obscene() != original->is_obscene()) <<
                (changed->quote() != original->quote()) <<
                (changed->owner() != original->owner()) <<
                (changed->id() == original->id()) <<
                (changed->cites() == original->cites());

    QString mapping = "/inside/update-quote";
    QNetworkRequest quoteRequest;

    QString bearerToken = "Bearer " + _accessToken;
    quoteRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    quoteRequest.setUrl(QUrl(_source + mapping));

    quoteRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body
        {
            {"quote", changed->serializeToQJsonDocument().object()},
            {"attrsChanged", attrsChanged},
            {"featuresChanged", featuresChanged},
            {"quoteChanged", quoteChanged}
        };

    qDebug() << body;
    QByteArray jsonQuote = QJsonDocument(body).toJson();

    QNetworkReply* reply = _manager->put(quoteRequest, jsonQuote);

    return reply;
}

QNetworkReply *QuotesApi::deleteQuote(int id)
{
    QString mapping = "/inside/delete-quote";
    QNetworkRequest quoteRequest;

    QUrl url(_source + mapping);

    QUrlQuery param;
    param.addQueryItem("id", QString::number(id));
    url.setQuery(param);

    QString bearerToken = "Bearer " + _accessToken;
    quoteRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    quoteRequest.setUrl(url);

    quoteRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = _manager->deleteResource(quoteRequest);

    return reply;
}

QNetworkReply *QuotesApi::getCoreTable(CoreTables ct)
{
    QString mapping = "/quotes/core-table";
    QString table;
    QNetworkRequest getTable;

    switch(ct)
    {
    case CoreTables::attrs:
        table = "attrs"; break;

    case CoreTables::features:
        table = "features"; break;

    case CoreTables::owners:
        table = "owners"; break;
    }

    QUrlQuery param;
    param.addQueryItem("table", table);

    QUrl url = QUrl(_source + mapping);
    url.setQuery(param);

    getTable.setUrl(url);

    QString bearerToken = "Bearer " + _accessToken;
    getTable.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    QNetworkReply* reply = _manager->get(getTable);

    return reply;
}

QNetworkReply *QuotesApi::getQuoteCards(QString owner)
{
    QString mapping = "/quotes/owners";
    QNetworkRequest getTable;

    QUrlQuery param;
    param.addQueryItem("owner", owner);

    QUrl url = QUrl(_source + mapping);
    url.setQuery(param);

    getTable.setUrl(url);

    QString bearerToken = "Bearer " + _accessToken;
    getTable.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    QNetworkReply* reply = _manager->get(getTable);

    return reply;
}

QNetworkReply *QuotesApi::getFavouriteCards()
{
    QString mapping = "/inside/fav-quotes";
    QNetworkRequest getTable;

    QUrlQuery param;

    QUrl url = QUrl(_source + mapping);
    url.setQuery(param);

    getTable.setUrl(url);

    QString bearerToken = "Bearer " + _accessToken;
    getTable.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    QNetworkReply* reply = _manager->get(getTable);

    return reply;
}

QNetworkReply *QuotesApi::addFavouriteCard(SingleQuoteModel* quote)
{
    QString mapping = "/inside/add-fav";
    QNetworkRequest quoteRequest;

    QString bearerToken = "Bearer " + _accessToken;
    quoteRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());

    quoteRequest.setUrl(QUrl(_source + mapping));

    quoteRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc = quote->serializeToQJsonDocument();

    QByteArray jsonQuote = doc.toJson();
    qDebug() << doc;
    qDebug() << jsonQuote;

    QNetworkReply* reply = _manager->put(quoteRequest, jsonQuote);

    return reply;
}

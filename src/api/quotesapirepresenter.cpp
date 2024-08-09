#include "quotesapirepresenter.h"
#include "qeventloop.h"
#include "qjsonobject.h"
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QException>
#include <QJsonArray>
#include <chrono>

QuotesApiRepresenter::QuotesApiRepresenter(): QObject(nullptr)
{
    _authTimer = new QTimer(this);
}

QuotesApiRepresenter::QuotesApiRepresenter(std::shared_ptr<Connection> conn, std::shared_ptr<User> user)
{

}

QuotesApiRepresenter::~QuotesApiRepresenter()
{
    qDebug() << "~QuotesApiRepresenter()";
    _authTimer->deleteLater();
}

void QuotesApiRepresenter::getCoreTable(QuotesApi::CoreTables ct)
{

    QNetworkReply* reply = _api->getCoreTable(ct);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) {qDebug() << "getCoreTable fucked up" << reply->errorString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute); throw QException(); }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (!doc.isArray()) {qDebug() << "its not array!!" << doc.isEmpty() << doc.isNull(); throw QException(); }

    QStringList* values = new QStringList(doc.toVariant().toStringList());

    switch (ct)
    {
    case QuotesApi::owners:
        emit sendOwnersValues(values);
    case QuotesApi::attrs:
        emit sendAttrsValues(values);
    case QuotesApi::features:
        emit sendFeaturesValues(values);
        break;
    }
}

void QuotesApiRepresenter::auth(std::shared_ptr<User> user, std::shared_ptr<Connection> conn)
{   
    _api = std::make_unique<QuotesApi>(conn, user);
    qDebug() << "QuotesApiRepresenter::QuotesApiRepresenter";
    _api->setParent(this);

    connect(_authTimer, &QTimer::timeout, _api.get(), &QuotesApi::authenticate);
    _lifetime = _api->authenticate();

    _authTimer->start(std::chrono::milliseconds(_lifetime - _lifetime / 10));
    qDebug() << _authTimer->interval();
}

void QuotesApiRepresenter::tryAuthenticate(std::shared_ptr<User> user, std::shared_ptr<Connection> conn)
{
    auth(user, conn);
    
    _lifetime != 0? emit authenticated() : emit forbidden();
}

void QuotesApiRepresenter::addUser(User* user)
{

    qDebug() << "QuotesApiRepresenter::addUser, user: " << user->username() << user->passwd();
    QNetworkReply* reply = _api->addUser(user);

    waitAnswer(reply);

    QByteArray responseBytes = reply->readAll();

    QString success = QString::fromUtf8(responseBytes).trimmed();

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit addedUserAnswer(success);
}

void QuotesApiRepresenter::getQuoteCards(const QString &owner)
{
    QNetworkReply* reply = _api->getQuoteCards(owner);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) throw QException();

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (!doc.isArray()) throw QException();
    QJsonArray raw = doc.array();
    QList<std::shared_ptr<SingleQuoteModel>>* values = new QList<std::shared_ptr<SingleQuoteModel>>();

    for(const QJsonValue & v: raw)
    {
        std::shared_ptr<SingleQuoteModel> q =
            std::shared_ptr<SingleQuoteModel>(SingleQuoteModel::buildFromQVariant(v.toObject().toVariantMap()));
        values->append(q);
    }

    delete reply;

    emit sendQuoteOwnerCards(values);
}

void QuotesApiRepresenter::getFavouriteCards()
{
    QNetworkReply* reply = _api->getFavouriteCards();

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) throw QException();

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (!doc.isArray()) throw QException();
    QJsonArray raw = doc.array();
    QList<std::shared_ptr<SingleQuoteModel>>* values = new QList<std::shared_ptr<SingleQuoteModel>>();

    for(const QJsonValue & v: raw)
    {
        std::shared_ptr<SingleQuoteModel> q =
            std::shared_ptr<SingleQuoteModel>(SingleQuoteModel::buildFromQVariant(v.toObject().toVariantMap()));
        values->append(q);
    }

    delete reply;

    emit sendQuoteFavouriteCards(values);
}

void QuotesApiRepresenter::addFavorite(SingleQuoteModel* quote)
{
    QNetworkReply* reply = _api->addFavouriteCard(quote);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) emit sendIfQuoteAddedToCollection(false);

    QByteArray responseBytes = reply->readAll();

    bool success = QString::fromUtf8(responseBytes).trimmed() == "true";

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit sendIfQuoteAddedToCollection(success);
}

void QuotesApiRepresenter::addQuote(SingleQuoteModel *data)
{
    qDebug() << "QuotesApiRepresenter::addQuote";
    QNetworkReply* reply = _api->addQuote(data);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) {emit sendIfQuoteAddedResponse(false); return;}

    QByteArray responseBytes = reply->readAll();

    bool success = QString::fromUtf8(responseBytes).trimmed() == "true";

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit sendIfQuoteAddedResponse(success);
}

void QuotesApiRepresenter::deleteQuote(SingleQuoteModel *data)
{
    qDebug() << "QuotesApiRepresenter::deleteQuote";
    QNetworkReply* reply = _api->deleteQuote(data->id());

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) {emit sendIfQuoteDeletedResponse(false); return;}

    QByteArray responseBytes = reply->readAll();

    bool success = QString::fromUtf8(responseBytes).trimmed() == "true";

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit sendIfQuoteDeletedResponse(success);
}

void QuotesApiRepresenter::addOwner(std::pair<QString, QString> owner)
{
    QNetworkReply* reply = _api->addOwner(owner);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) {emit sendIfOwnerAddedResponse(false); return;}

    QByteArray responseBytes = reply->readAll();

    bool success = QString::fromUtf8(responseBytes).trimmed() == "true";

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit sendIfOwnerAddedResponse(success);
}

void QuotesApiRepresenter::updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original)
{
    QNetworkReply* reply = _api->updateQuote(changed, original);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) {emit sendIfQuoteUpdatedResponse("Some error"); return;}

    QByteArray responseBytes = reply->readAll();

    QString success = QString::fromUtf8(responseBytes).trimmed();

    qDebug() << responseBytes << success << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    delete reply;

    emit sendIfQuoteUpdatedResponse(success);
}

void QuotesApiRepresenter::searchQuote(const QString& quote)
{
    QNetworkReply* reply = _api->searchQuote(quote);

    waitAnswer(reply);
    if (reply->error() != QNetworkReply::NoError) throw QException();

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if (!doc.isArray()) throw QException();
    QJsonArray raw = doc.array();
    QList<std::shared_ptr<SingleQuoteModel>>* values = new QList<std::shared_ptr<SingleQuoteModel>>();

    for(const QJsonValue & v: raw)
    {
        std::shared_ptr<SingleQuoteModel> q =
            std::shared_ptr<SingleQuoteModel>(SingleQuoteModel::buildFromQVariant(v.toObject().toVariantMap()));
        values->append(q);
    }

    delete reply;

    emit searchQuoteResponse(values);
}

void QuotesApiRepresenter::waitAnswer(QNetworkReply *reply)
{
    QEventLoop loop(this);
    QAbstractSocket::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

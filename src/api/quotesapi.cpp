#include "quotesapi.h"
#include "qeventloop.h"
#include "qurl.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

QuotesApi::QuotesApi(std::shared_ptr<Connection> conn, std::shared_ptr<User> user)
{
    _authenticate = new QuotesLogApi(conn, user);
}

QuotesApi::~QuotesApi()
{
    delete _authenticate;
}

QNetworkReply *QuotesApi::addQuote(SingleQuoteModel *data)
{
    QString mapping = "/inside/add-quote";

    ANY_REQUEST_AUTHED_TEMPLATE
    
    QJsonDocument doc = data->serializeToQJsonDocument();

    QByteArray jsonQuote = doc.toJson();
    qDebug() << doc;
    qDebug() << jsonQuote;

    QNetworkReply* reply = _authenticate->_manager->post(quoteRequest, jsonQuote);

    return reply;
}

QNetworkReply *QuotesApi::addOwner(std::pair<QString, QString> owner)
{
    QString mapping = "/inside/add-owner";

    ANY_REQUEST_AUTHED_TEMPLATE
    QJsonObject json
        {
            {"owner", owner.first},
            {"description", owner.second}
        };

    QJsonDocument doc(json);
    QByteArray jsonOwner = doc.toJson();

    QNetworkReply* reply = _authenticate->_manager->post(quoteRequest, jsonOwner);
    return reply;
}

QNetworkReply *QuotesApi::updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original)
{

    assert((changed->id() == original->id()) && (changed->cites() == original->cites()));
    bool attrsChanged    = changed->attrs() != original->attrs();
    bool featuresChanged = changed->features() != original->features();
    bool quoteChanged    = (changed->is_obscene() != original->is_obscene()) ||
                           (changed->quote() != original->quote()) ||
                           (changed->owner() != original->owner());

    // qDebug() << changed->is_obscene() << original->is_obscene() <<
    //             changed->quote() << original->quote() <<
    //             changed->owner() << original->owner() <<
    //             changed->id() << original->id() <<
    //             changed->cites() << original->cites();

    // qDebug() << (changed->is_obscene() != original->is_obscene()) <<
    //             (changed->quote() != original->quote()) <<
    //             (changed->owner() != original->owner()) <<
    //             (changed->id() == original->id()) <<
    //             (changed->cites() == original->cites());

    QString mapping = "/inside/update-quote";
    ANY_REQUEST_AUTHED_TEMPLATE

    QJsonObject body
        {
            {"quote", changed->serializeToQJsonDocument().object()},
            {"attrsChanged", attrsChanged},
            {"featuresChanged", featuresChanged},
            {"quoteChanged", quoteChanged}
        };

    qDebug() << body;
    QByteArray jsonQuote = QJsonDocument(body).toJson();

    QNetworkReply* reply = _authenticate->_manager->put(quoteRequest, jsonQuote);

    return reply;
}

QNetworkReply *QuotesApi::deleteQuote(int id)
{
    QString mapping = "/inside/delete-quote";
    
    ANY_REQUEST_AUTHED_TEMPLATE

    QUrlQuery param;
    param.addQueryItem("id", QString::number(id));
    QURL_PARAM

    QNetworkReply* reply = _authenticate->_manager->deleteResource(quoteRequest);

    return reply;
}

QNetworkReply *QuotesApi::getCoreTable(CoreTables ct)
{
    QString mapping = "/quotes/core-table";

    ANY_REQUEST_AUTHED_TEMPLATE

    QString table;

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
    QURL_PARAM
    qDebug() << "getCore Table" << url.toString();
    QNetworkReply* reply = _authenticate->_manager->get(quoteRequest);

    return reply;
}

QNetworkReply *QuotesApi::getQuoteCards(QString owner)
{
    QString mapping = "/quotes/owners";
    ANY_REQUEST_AUTHED_TEMPLATE

    QUrlQuery param;
    param.addQueryItem("owner", owner);
    QURL_PARAM

    QNetworkReply* reply = _authenticate->_manager->get(quoteRequest);

    return reply;
}

QNetworkReply *QuotesApi::getFavouriteCards()
{
    QString mapping = "/inside/fav-quotes";
    ANY_REQUEST_AUTHED_TEMPLATE

    QNetworkReply* reply = _authenticate->_manager->get(quoteRequest);

    return reply;
}

QNetworkReply *QuotesApi::addFavouriteCard(SingleQuoteModel* quote)
{
    QString mapping = "/inside/add-fav";
    ANY_REQUEST_AUTHED_TEMPLATE
    
    QJsonDocument doc = quote->serializeToQJsonDocument();

    QByteArray jsonQuote = doc.toJson();
    qDebug() << doc;
    qDebug() << jsonQuote;

    QNetworkReply* reply = _authenticate->_manager->put(quoteRequest, jsonQuote);

    return reply;
}

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

API_METHOD(addQuote, "/inside/add-quote", SingleQuoteModel *data)    
    QJsonDocument doc = data->serializeToQJsonDocument();
    QByteArray body = doc.toJson();
BODY_REQUEST(post)

API_METHOD(addOwner, "/inside/add-owner", std::pair<QString, QString> owner)
    QJsonObject json
        {
            {"owner", owner.first},
            {"description", owner.second}
        };
    QJsonDocument doc(json);
    QByteArray body = doc.toJson();
BODY_REQUEST(post)

API_METHOD(updateQuote, "/inside/update-quote", SingleQuoteModel* changed, SingleQuoteModel* original)
    assert((changed->id() == original->id()) && (changed->cites() == original->cites()));
    bool attrsChanged    = changed->attrs() != original->attrs();
    bool featuresChanged = changed->features() != original->features();
    bool quoteChanged    = (changed->is_obscene() != original->is_obscene()) ||
                           (changed->quote() != original->quote()) ||
                           (changed->owner() != original->owner());
    QJsonObject json
        {
            {"quote", changed->serializeToQJsonDocument().object()},
            {"attrsChanged", attrsChanged},
            {"featuresChanged", featuresChanged},
            {"quoteChanged", quoteChanged}
        };
    QByteArray body = QJsonDocument(json).toJson();
BODY_REQUEST(put)

API_METHOD(deleteQuote, "/inside/delete-quote", int id)
    QUrlQuery param;
    param.addQueryItem("id", QString::number(id));
NO_BODY_REQUEST(deleteResource)


API_METHOD(getCoreTable, "/quotes/core-table", CoreTables ct)
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
NO_BODY_REQUEST(get)

API_METHOD(getQuoteCards, "/quotes/owners", QString owner)
    QUrlQuery param;
    param.addQueryItem("owner", owner);
NO_BODY_REQUEST(get)

API_METHOD(getOwnedCards, "/inside/owning", void)
QUrlQuery param;
NO_BODY_REQUEST(get)

API_METHOD(getFavouriteCards, "/inside/fav-quotes", void)
QUrlQuery param;
NO_BODY_REQUEST(get)

API_METHOD(addFavouriteCard, "/inside/add-fav", SingleQuoteModel* quote)
    QJsonDocument doc = quote->serializeToQJsonDocument();
    QByteArray body = doc.toJson();
BODY_REQUEST(put)

API_METHOD(removeFavouriteCard, "/inside/remove-fav", int quoteId)
    QUrlQuery param;
    param.addQueryItem("quoteId", QString::number(quoteId));
NO_BODY_REQUEST(deleteResource)

API_METHOD(searchQuote, "/quotes/search", const QString& quote)
    QUrlQuery param;
    param.addQueryItem("quote", quote);
NO_BODY_REQUEST(get)

API_METHOD(searchQuote, "/quotes/search", const QString& quote, const QStringList* features, bool negative)
    QUrlQuery param;
    param.addQueryItem("quote", quote);
    for (auto& feature : *features) 
    {
        param.addQueryItem("features", feature);
    }
    param.addQueryItem("negative", negative? "true" : "false");
NO_BODY_REQUEST(get)
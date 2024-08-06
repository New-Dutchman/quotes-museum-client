#ifndef QUOTESAPI_H
#define QUOTESAPI_H

#include <QObject>

#include "qurl.h"
#include <QString>
#include <QNetworkAccessManager>

#include "quoteslogapi.h"

class QuotesApi: public QObject
{
    Q_OBJECT

public:
    QuotesApi(std::shared_ptr<Connection> _conn, std::shared_ptr<User> _user);
    ~QuotesApi();

    enum CoreTables{owners, attrs, features};

    QNetworkReply* getCoreTable(CoreTables);
    QNetworkReply* getQuoteCards(QString);
    QNetworkReply* getFavouriteCards();
    QNetworkReply* addFavouriteCard(SingleQuoteModel* quote);
    QNetworkReply* addQuote(SingleQuoteModel* data);
    QNetworkReply* deleteQuote(int id);
    QNetworkReply* addOwner(std::pair<QString, QString>);
    QNetworkReply* updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original);

    inline int authenticate() {return _authenticate->auth(); };
    inline QNetworkReply* addUser(User * user) {return _authenticate->addUser(user); };

private:
    QuotesLogApi* _authenticate;

    QNetworkAccessManager& _manager = *_authenticate->_manager;
    // internal use only
    #define ANY_REQUEST_AUTHED_TEMPLATE                                                 \
        QNetworkRequest quoteRequest;                                                   \
        QString bearerToken = "Bearer " + _authenticate->_accessToken;                  \
        quoteRequest.setRawHeader("Authorization", bearerToken.toLocal8Bit());          \
        quoteRequest.setUrl(QUrl(_authenticate->_source + mapping));                    \
        quoteRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");     
};

#endif // QUOTESAPI_H

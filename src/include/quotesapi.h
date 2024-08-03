#ifndef QUOTESAPI_H
#define QUOTESAPI_H

#include "qurl.h"
#include <QString>
#include <QNetworkAccessManager>

#include "connection.h"
#include "singlequotemodel.h"
#include "user.h"

class QuotesApi: public QObject
{
    Q_OBJECT

public:
    QuotesApi(std::shared_ptr<Connection> _conn, std::shared_ptr<User> _user);
    virtual ~QuotesApi();

    enum CoreTables{owners, attrs, features};

    int auth();

    QNetworkReply* getCoreTable(CoreTables);
    QNetworkReply* getQuoteCards(QString);
    QNetworkReply* getFavouriteCards();
    QNetworkReply* addFavouriteCard(SingleQuoteModel* quote);
    QNetworkReply* addUser(User *);
    QNetworkReply* addQuote(SingleQuoteModel* data);
    QNetworkReply* deleteQuote(int id);
    QNetworkReply* addOwner(std::pair<QString, QString>);
    QNetworkReply* updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original);

private:
    QString _source;
    std::shared_ptr<Connection> _conn;
    std::shared_ptr<User> _user;

    QNetworkAccessManager* _manager;
    QString _accessToken;

    //QNetworkRequest buildRequest(QString mapping);
};

#endif // QUOTESAPI_H

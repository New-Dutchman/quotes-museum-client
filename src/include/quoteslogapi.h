#ifndef __QUOTES_LOG_API_H__
#define __QUOTES_LOG_API_H__

#include <QObject>

#include "qurl.h"
#include <QString>
#include <QNetworkAccessManager>

#include "connection.h"
#include "singlequotemodel.h"
#include "user.h"
#include "quotesapi.h"

class QuotesLogApi: public QObject
{
    Q_OBJECT

    friend class QuotesApi;

    QuotesLogApi(std::shared_ptr<Connection> _conn, std::shared_ptr<User> _user);
    virtual ~QuotesLogApi();

    std::shared_ptr<Connection> _conn;
    std::shared_ptr<User> _user;

    QString _source;

    QNetworkAccessManager* _manager;
    QString _accessToken;

public:
    int auth();

    QNetworkReply* addUser();

};

#endif // __QUOTES_LOG_API_H__

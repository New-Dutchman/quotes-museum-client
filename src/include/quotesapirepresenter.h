#ifndef QUOTESAPIREPRESENTER_H
#define QUOTESAPIREPRESENTER_H

#include <Qtimer>
#include <QStringList>

#include "quotesapi.h"
#include "singlequotemodel.h"

class QuotesApiRepresenter: public QObject
{
    Q_OBJECT

public:
    QuotesApiRepresenter();
    // QuotesApiRepresenter(std::shared_ptr<Connection> conn, std::shared_ptr<User> user);
    ~QuotesApiRepresenter();

public slots:
    void tryAuthenticate(std::shared_ptr<User> user, std::shared_ptr<Connection> conn);
    void addUser(std::shared_ptr<User> user, std::shared_ptr<Connection> conn);

signals:
    void authenticated();
    void forbidden();
    void addedUserAnswer(QString);

    void sendOwnersValues(QStringList*);
    void sendAttrsValues(QStringList*);
    void sendFeaturesValues(QStringList*);

    void sendQuoteOwnerCards(QList<std::shared_ptr<SingleQuoteModel>>*);
    void sendQuoteFavouriteCards(QList<std::shared_ptr<SingleQuoteModel>>*);
    void sendQuotePossessedCards(QList<std::shared_ptr<SingleQuoteModel>>*);

    void sendFavouriteQuoteStatus(bool, const QString&);
    void sendIfQuoteAddedResponse(bool);
    void sendIfQuoteDeletedResponse(bool);
    void sendIfOwnerAddedResponse(bool);
    void sendIfQuoteUpdatedResponse(QString);

    void searchQuoteResponse(QList<std::shared_ptr<SingleQuoteModel>>* quotes);

public slots:
    void getCoreTable(QuotesApi::CoreTables);
    void getQuoteCards(const QString &owner);
    
    void getPossessedCards();
    void getFavouriteCards();
    void addFavorite(SingleQuoteModel* quote);
    void removeFavorite(SingleQuoteModel* quote);

    void addQuote(SingleQuoteModel* data);
    void deleteQuote(SingleQuoteModel* data);
    void addOwner(std::pair<QString, QString>);
    void updateQuote(SingleQuoteModel *changed, SingleQuoteModel *original);

    void searchQuote(const QString&);
    void searchQuoteWithFilter(const QString&, const QStringList* filters, bool negative);

private:
    std::unique_ptr<QuotesApi> _api;
    QString _accessToken;
    uint _lifetime;
    QTimer *_authTimer;

    void waitAnswer(QNetworkReply *reply);
    void auth(std::shared_ptr<User> user, std::shared_ptr<Connection> conn);
};

#endif // QUOTESAPIREPRESENTER_H

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
    QuotesApiRepresenter(std::shared_ptr<QuotesApi> _api);
    ~QuotesApiRepresenter();

    bool authed();
    QString addUser(User*);

signals:
    void sendOwnersValues(QStringList*);
    void sendAttrsValues(QStringList*);
    void sendFeaturesValues(QStringList*);

    void sendQuoteOwnerCards(QList<std::shared_ptr<SingleQuoteModel>>*);
    void sendQuoteFavouriteCards(QList<std::shared_ptr<SingleQuoteModel>>*);

    void sendIfQuoteAddedToCollection(bool);
    void sendIfQuoteAddedResponse(bool);
    void sendIfQuoteDeletedResponse(bool);
    void sendIfOwnerAddedResponse(bool);
    void sendIfQuoteUpdatedResponse(QString);

public slots:
    void getCoreTable(QuotesApi::CoreTables);
    void getQuoteCards(const QString &owner);

    void getFavouriteCards();
    void addFavorite(SingleQuoteModel* quote);

    void addQuote(SingleQuoteModel* data);
    void deleteQuote(SingleQuoteModel* data);
    void addOwner(std::pair<QString, QString>);
    void updateQuote(SingleQuoteModel* changed, SingleQuoteModel* original);


private:
    std::shared_ptr<QuotesApi> _api;
    QString _accessToken;
    uint _lifetime;
    QTimer* _authTimer;

    void auth();
};

#endif // QUOTESAPIREPRESENTER_H

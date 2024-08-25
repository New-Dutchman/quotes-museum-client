#ifndef AUTHEDQUOTES_H
#define AUTHEDQUOTES_H

#include <QTabWidget>
#include <QThread>

#include "quotesapirepresenter.h"
#include "searchtab.h"
#include "clickablelabel.h"

namespace Ui {
class AuthedQuotes;
}

class AuthedQuotes : public QTabWidget
{
    Q_OBJECT

public:
    explicit AuthedQuotes(std::shared_ptr<QuotesApiRepresenter>, QString username, QWidget *parent = nullptr);

    ~AuthedQuotes();

signals:
    void sendGetCoreTableRequest(QuotesApi::CoreTables);

    void addFavouriteQuoteRequest(SingleQuoteModel* model);
    void removeFavouriteQuoteRequest(SingleQuoteModel* model);
    void sendAddQuoteRequest(SingleQuoteModel* model);
    void sendDeleteQuoteRequest(SingleQuoteModel* model);
    void sendAddOwnerRequest(std::pair<QString, QString> data);
    void sendUpdateQuoteRequest(SingleQuoteModel* data, SingleQuoteModel* original);
    void sendUpdateAddedQuotesRequest();
    void sendUpdateFavsQuotesRequest();


public slots:
    void favouriteQuoteFromSingleCard(SingleQuoteModel* model);
    void updateQuoteFromSingleCard(SingleQuoteModel* model);
    void aboutToDeleteQuoteFromSingleCard(SingleQuoteModel* model);
    void removeFavouriteQuoteFromSingleCard(SingleQuoteModel* model);

    void quoteUpdateAccepted(SingleQuoteModel* data, SingleQuoteModel* original);
    void quoteAddAccepted(SingleQuoteModel* data);

    void addOwnerAccepted(std::pair<QString, QString>);

private slots:
    void onAddQuoteClicked();
    void onAddOwnerClicked();

    void setOwners(QStringList *owners);
    void setAttrs(QStringList *attrs);
    void setFeatures(QStringList *features);

    void onOwnersComdoBoxCurrentTextChanged(QList<std::shared_ptr<SingleQuoteModel>>* quotes);
    void onUpdateFavsClicked(QList<std::shared_ptr<SingleQuoteModel>>* quotes);
    void onUpdateAddedClicked(QList<std::shared_ptr<SingleQuoteModel>>* quotes);
    void updateAddedQuotesRequest();
    void changeLabelsState();
    void updateFavsQuotesRequest();

    // void addFavourite(bool);
    void favouriteQuoteAction(bool, const QString&);
    void addQuoteResponse(bool);
    void deleteQuoteResponse(bool);
    void addOwnerResponse(bool);
    void updateQuoteResponse(QString);

    void onUpdatePageClicked();

private:
    std::unique_ptr<SearchTab> searchTab;

    friend SearchTab;

    void createResponseWindow(QString, bool);

    Ui::AuthedQuotes *ui;

    void freeQuotesLayout(QLayout *l);
    void getCoreTable(QuotesApi::CoreTables);
    std::shared_ptr<QuotesApiRepresenter> _quotesApi;
    QStringList* _owners;
    QStringList* _attrs;
    QStringList* _features;

    ClickableLabel<AuthedQuotes>* favouriteQuotesLabel;
    ClickableLabel<AuthedQuotes>* addedQuotesLabel;

};

#endif // AUTHEDQUOTES_H

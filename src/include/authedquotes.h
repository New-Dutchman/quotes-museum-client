#ifndef AUTHEDQUOTES_H
#define AUTHEDQUOTES_H

#include <QTabWidget>
#include <QThread>

#include "quotesapirepresenter.h"

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
    void sendAddQuoteRequest(SingleQuoteModel* model);
    void sendDeleteQuoteRequest(SingleQuoteModel* model);
    void sendAddOwnerRequest(std::pair<QString, QString> data);
    void sendUpdateQuoteRequest(SingleQuoteModel* data, SingleQuoteModel* original);

public slots:
    void favouriteQuoteFromSingleCard(SingleQuoteModel* );
    void updateQuoteFromSingleCard(SingleQuoteModel* model);
    void aboutToDeleteQuoteFromSingleCard(SingleQuoteModel* model);

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
    void onUpdateBtnClicked(QList<std::shared_ptr<SingleQuoteModel>>* quotes);

    void addFavourite(bool);
    void addQuoteResponse(bool);
    void deleteQuoteResponse(bool);
    void addOwnerResponse(bool);
    void updateQuoteResponse(QString);

private:
    void createResponseWindow(QString, bool);

    Ui::AuthedQuotes *ui;

    QThread* _apiThread;

    void freeQuotesLayout(QLayout *l);
    void getCoreTable(QuotesApi::CoreTables);
    std::shared_ptr<QuotesApiRepresenter> _quotesApi;
    QStringList* _owners;
    QStringList* _attrs;
    QStringList* _features;
};

#endif // AUTHEDQUOTES_H

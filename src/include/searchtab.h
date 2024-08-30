#ifndef __SEARCH_TAB__
#define __SEARCH_TAB__

#include <QWidget>
#include "singlequotecard.h"
#include "quotesapirepresenter.h"

class AuthedQuotes;

namespace Ui {
class SearchTab;
}

class SearchTab : public QWidget
{
    Q_OBJECT

public:
    explicit SearchTab(std::shared_ptr<QuotesApiRepresenter>, AuthedQuotes *parent);
    ~SearchTab();

signals:
    void searchForQuoteRequest(const QString& quote);

public slots:
    void searchForQuoteResponse(QList<std::shared_ptr<SingleQuoteModel>>* quotes);

private slots:
    void sendSearchQuoteRequest();

private:
    std::shared_ptr<QuotesApiRepresenter> presenter;
    Ui::SearchTab* ui;
    AuthedQuotes* _aqPtr;

    void freeQuotesLayout(QLayout* l);
};

#endif // __SEARCH_TAB__
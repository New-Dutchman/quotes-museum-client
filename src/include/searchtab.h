#ifndef __SEARCH_TAB__
#define __SEARCH_TAB__

#include <QWidget>
#include "singlequotecard.h"
#include "quotesapirepresenter.h"

namespace Ui {
class SearchTab;
}

class SearchTab : public QWidget
{
    Q_OBJECT

public:
    explicit SearchTab(std::shared_ptr<QuotesApiRepresenter>, QWidget *parent = nullptr);
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
};

#endif // __SEARCH_TAB__
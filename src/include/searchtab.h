#ifndef __SEARCH_TAB__
#define __SEARCH_TAB__

#include <QWidget>
#include "singlequotecard.h"
#include "quotesapirepresenter.h"
#include <QVBoxLayout>
#include "checkablemenu.h"
#include "clickablelabel.h"

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
    void searchForQuoteRequestWithFilters(const QString& quote, const QStringList* filters, bool negative);
    void sendToUncheckMenu(const QString &title);

public slots:
    void searchForQuoteResponse(QList<std::shared_ptr<SingleQuoteModel>>* quotes);

private slots:
    void sendSearchQuoteRequest();
    void addChosenAttrs(const QString& attributes);

public slots:
    void removeAttr(ClickableLabel<SearchTab>* caller);
    void uncheckedAttr(const QString& attribute);

private:
    std::shared_ptr<QuotesApiRepresenter> presenter;
    Ui::SearchTab* ui;
    AuthedQuotes* _aqPtr;
    QStringList _groups;
    QMap<QString, QVBoxLayout*> _groupLayouts;
    CheckableMenu* _chooseAttrs;
    QStringList _chosenAttrs;
    QList<ClickableLabel<SearchTab>*> _attrsLabels;

    void freeQuotesLayout(QLayout* l);

    void emplaceQuoteToGroupFrame(SingleQuoteCard* quote, const QString& group);
};

#endif // __SEARCH_TAB__
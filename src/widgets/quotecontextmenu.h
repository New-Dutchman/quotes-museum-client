#ifndef __QUOTE_CONTEXT_MENU__
#define __QUOTE_CONTEXT_MENU__

#include "singlequotecard.h"

class SingleQuoteCard::QuoteContextMenu: public QMenu
{
    Q_OBJECT

public:
    explicit QuoteContextMenu(SingleQuoteCard* parent);
    ~QuoteContextMenu();

private:
    QAction* updateAction;
    QAction* deleteAction;
    QAction* favouriteAction;

    void setActionList();

    void (SingleQuoteCard::*favActionMethod)(void);

    SingleQuoteCard* _parent;
};

#endif // __QUOTE_CONTEXT_MENU__ 
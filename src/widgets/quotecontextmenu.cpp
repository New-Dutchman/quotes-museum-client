#include "quotecontextmenu.h"

#define Menu SingleQuoteCard::QuoteContextMenu

Menu::QuoteContextMenu(SingleQuoteCard* parent)
    : QMenu(parent)
    , _parent(parent)
    {
        setActionList();
        QObject::connect(updateAction, &QAction::triggered, _parent, &SingleQuoteCard::onUpdateClicked);
        QObject::connect(deleteAction, &QAction::triggered, _parent, &SingleQuoteCard::onDeletCardClicked);
        QObject::connect(favouriteAction, &QAction::triggered, _parent, favActionMethod);
    }

Menu::~QuoteContextMenu() { }

void Menu::setActionList()
{
    updateAction = addAction("Update Quote");
    deleteAction = addAction("Delete Quote");
    switch (_parent->options)
    {
        case QuoteMode::JustWatch:
            favouriteAction = addAction("Add to Favorites");
            favActionMethod = &SingleQuoteCard::onAddFavClicked;
            break;

        case QuoteMode::Favourite:
            favouriteAction = addAction("Remove from Favorites");
            favActionMethod = &SingleQuoteCard::onRemoveFavClicked;
            break;

        default:
            assert(false);
    }
}

#undef Menu
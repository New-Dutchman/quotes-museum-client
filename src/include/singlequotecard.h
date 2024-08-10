#ifndef SINGLEQUOTECARD_H
#define SINGLEQUOTECARD_H

#include "singlequotemodel.h"
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>

namespace Ui {
class SingleQuoteCard;
}

class SingleQuoteCard : public QWidget
{
    Q_OBJECT

public:
    enum class QuoteMode
    {
        JustWatch = 0,
        Favourite = 1
    };

    explicit SingleQuoteCard(std::shared_ptr<SingleQuoteModel> quote, QuoteMode mode, QWidget *parent = nullptr);
    ~SingleQuoteCard();

public: signals:
    void updateInfo(SingleQuoteModel* model);
    void addFavourite(SingleQuoteModel* model);
    void deleteCard(SingleQuoteModel* model);
    void removeFavourite(SingleQuoteModel* model);

private slots:
    void onUpdateClicked();
    void onAddFavClicked();
    void onDeletCardClicked();
    void onRemoveFavClicked();

private:
    Ui::SingleQuoteCard *ui;
    std::shared_ptr<SingleQuoteModel> _model;

    QuoteMode options;
    class QuoteContextMenu;

    QuoteContextMenu* quoteMenu;

protected:
void mousePressEvent(QMouseEvent *event) override;

};

#endif // SINGLEQUOTECARD_H

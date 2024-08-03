#ifndef SINGLEQUOTECARD_H
#define SINGLEQUOTECARD_H

#include "singlequotemodel.h"
#include <QWidget>

namespace Ui {
class SingleQuoteCard;
}

class SingleQuoteCard : public QWidget
{
    Q_OBJECT

public:
    explicit SingleQuoteCard(std::shared_ptr<SingleQuoteModel> quote, QWidget *parent = nullptr);
    ~SingleQuoteCard();

public: signals:
    void updateInfo(SingleQuoteModel* model);
    void addFavourite(SingleQuoteModel* model);
    void deleteCard(SingleQuoteModel* model);

private slots:
    void onUpdateClicked();
    void onAddFavClicked();
    void onDeletCardClicked();

private:
    Ui::SingleQuoteCard *ui;
    std::shared_ptr<SingleQuoteModel> _model;
};

#endif // SINGLEQUOTECARD_H

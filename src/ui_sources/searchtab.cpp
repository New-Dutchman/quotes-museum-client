#include "searchtab.h"
#include "ui_searchtab.h"
#include "authedquotes.h"

SearchTab::SearchTab(std::shared_ptr<QuotesApiRepresenter> representer, AuthedQuotes *parent)
    : QWidget(parent)
    , ui(new Ui::SearchTab)
    , presenter(representer)
    , _aqPtr(parent)
{
    ui->setupUi(this);

    QObject::connect(ui->searchQuoteBtn, &QPushButton::clicked, this, &SearchTab::sendSearchQuoteRequest);
    QObject::connect(this, &SearchTab::searchForQuoteRequest, representer.get(), &QuotesApiRepresenter::searchQuote);
    QObject::connect(representer.get(), &QuotesApiRepresenter::searchQuoteResponse, this, &SearchTab::searchForQuoteResponse);
}

SearchTab::~SearchTab()
{
    delete ui;
}

void SearchTab::searchForQuoteResponse(QList<std::shared_ptr<SingleQuoteModel>>* quotes)
{
    freeQuotesLayout(ui->quotesLayout);
    if (quotes->size() == 0)
    {
        ui->startText->show();
        return;
    }

    foreach(const std::shared_ptr<SingleQuoteModel> &sqm, *quotes)
    {
        QWidget *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::JustWatch, _aqPtr);
        ui->quotesLayout->addWidget(sqc);
    }

    delete quotes;
}

void SearchTab::sendSearchQuoteRequest()
{
    ui->startText->hide();
    emit searchForQuoteRequest(ui->quoteLE->text());
}

void SearchTab::freeQuotesLayout(QLayout* l)
{
    QLayoutItem* item;
    while ((item = l->takeAt(0)) != NULL )
    {
        if (item->widget() == ui->startText)
            continue;
        delete item->widget();
        delete item;
    }
}
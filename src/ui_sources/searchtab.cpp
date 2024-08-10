#include "searchtab.h"
#include "ui_searchtab.h"
#include "authedquotes.h"

SearchTab::SearchTab(std::shared_ptr<QuotesApiRepresenter> representer, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchTab)
    , presenter(representer)
{
    ui->setupUi(this);

    QObject::connect(ui->searchQuoteBtn, &QPushButton::clicked, this, &SearchTab::sendSearchQuoteRequest);
    QObject::connect(this, &SearchTab::searchForQuoteRequest, representer.get(), &QuotesApiRepresenter::searchQuote);
    QObject::connect(representer.get(), &QuotesApiRepresenter::searchQuoteResponse, this, &SearchTab::searchForQuoteResponse);


    QObject::connect(ui->searchQuoteBtn, &QPushButton::clicked, this, &SearchTab::sendSearchQuoteRequest);
}

SearchTab::~SearchTab()
{
    delete ui;
}

void SearchTab::searchForQuoteResponse(QList<std::shared_ptr<SingleQuoteModel>>* quotes)
{
    dynamic_cast<AuthedQuotes*>(this->parent())->freeQuotesLayout(ui->quotesLayout);
    if (quotes == nullptr)
    {
        ui->startText->show();
        return;
    } 

    foreach(const std::shared_ptr<SingleQuoteModel> &sqm, *quotes)
    {
        QWidget *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::JustWatch, this);
        ui->quotesLayout->addWidget(sqc);
    }

    delete quotes;
}

void SearchTab::sendSearchQuoteRequest()
{
    emit searchForQuoteRequest(ui->quoteLE->text());
}
#include "singlequotecard.h"
#include "authedquotes.h"
#include "ui_singlequotecard.h"
#include "quotecontextmenu.h"

SingleQuoteCard::SingleQuoteCard(std::shared_ptr<SingleQuoteModel> quote, QuoteMode mode, AuthedQuotes *parent)
    : QWidget(parent)
    , options(mode)
    , ui(new Ui::SingleQuoteCard)
{
    ui->setupUi(this);
    _model = quote;

    ui->quoteLabel->setText(_model->quote());
    ui->label->setText("cites: " + QString::number(_model->cites()));
    //this->setStyleSheet("border: 2px rgb(0, 255, 127)");
    //ui->frame->setStyleSheet("border: 2px solid rgb(0, 211, 158);");

    foreach(const QString &text, _model->features())
    {
        QLabel* l = new QLabel(this);
        l->setText(text);
        l->setFont(QFont("Segoe UI", 10));
        ui->featuresLayout->addWidget(l);
    }

    for (auto [key, value] : _model->attrs().asKeyValueRange())
    {
        QLabel* l = new QLabel(this);
        l->setText(key + ": " + value);
        // qDebug() << attrs.first + ": " + attrs.second;
        l->setFont(QFont("Segoe UI", 10));
        ui->attrsLayout->addWidget(l);
    }
    
    AuthedQuotes* aq = static_cast<AuthedQuotes*>(this->parent());
    // QObject::connect(ui->updateQuoteBtn, &QPushButton::clicked, this, &SingleQuoteCard::onUpdateClicked);
    QObject::connect(this, &SingleQuoteCard::updateInfo, aq, &AuthedQuotes::updateQuoteFromSingleCard);

    // QObject::connect(ui->addFavsBtn, &QPushButton::clicked, this, &SingleQuoteCard::onAddFavClicked);
    QObject::connect(this, &SingleQuoteCard::addFavourite, aq, &AuthedQuotes::favouriteQuoteFromSingleCard);

    // QObject::connect(ui->deleteCardBtn, &QPushButton::clicked, this, &SingleQuoteCard::onDeletCardClicked);
    QObject::connect(this, &SingleQuoteCard::deleteCard, aq, &AuthedQuotes::aboutToDeleteQuoteFromSingleCard);

    QObject::connect(this, &SingleQuoteCard::removeFavourite, aq, &AuthedQuotes::removeFavouriteQuoteFromSingleCard);

}

SingleQuoteCard::~SingleQuoteCard()
{
    delete ui;
}

void SingleQuoteCard::onUpdateClicked()
{
    qDebug() << "SingleQuoteCard::onUpdateClicked";

    AuthedQuotes* aq = static_cast<AuthedQuotes*>(this->parent());

    emit updateInfo(_model.get());
}

void SingleQuoteCard::onAddFavClicked()
{
    //qDebug() << "SingleQuoteCard::onAddFavClicked";
    emit addFavourite(_model.get());
}

void SingleQuoteCard::onRemoveFavClicked()
{
    emit removeFavourite(_model.get());
}

void SingleQuoteCard::onDeletCardClicked()
{
    qDebug() << "SingleQuoteCard::onDeletCardClicked";
    emit deleteCard(_model.get());
}

void SingleQuoteCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) 
    {
        quoteMenu = new QuoteContextMenu(this);

        quoteMenu->popup(event->globalPosition().toPoint());
    }
    QWidget::mousePressEvent(event);
}

#include "searchtab.h"
#include "ui_searchtab.h"
#include "authedquotes.h"
#include <QGroupBox>
#include <QStringLiteral>

SearchTab::SearchTab(std::shared_ptr<QuotesApiRepresenter> representer, AuthedQuotes *parent)
    : QWidget(parent)
    , ui(new Ui::SearchTab)
    , presenter(representer)
    , _aqPtr(parent)
{
    ui->setupUi(this);
    const QStringList* feats = parent->getFeatures();
    // int columnCount = attrs->length() / 3;
    ui->chosenAttrsLayout->setDefaultPositioning(3, Qt::Orientation::Horizontal);
    _chooseAttrs = new CheckableMenu("choose features", feats, this);
    ui->optionsLayout->insertWidget(0, _chooseAttrs);

    QObject::connect(ui->searchQuoteBtn, &QPushButton::clicked, this, &SearchTab::sendSearchQuoteRequest);
    QObject::connect(ui->quoteLE, &QLineEdit::returnPressed, this, &SearchTab::sendSearchQuoteRequest);
    QObject::connect(this, &SearchTab::searchForQuoteRequest, representer.get(), &QuotesApiRepresenter::searchQuote);
    QObject::connect(representer.get(), &QuotesApiRepresenter::searchQuoteResponse, this, &SearchTab::searchForQuoteResponse);

    QObject::connect(_chooseAttrs, &CheckableMenu::actionChosen, this, &SearchTab::addChosenAttrs);
    QObject::connect(_chooseAttrs, &CheckableMenu::actionChosenOff, this, &SearchTab::uncheckedAttr);
    QObject::connect(this, &SearchTab::sendToUncheckMenu, _chooseAttrs, &CheckableMenu::uncheck);
    
}

SearchTab::~SearchTab()
{
    delete ui;
    delete _chooseAttrs;
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
        SingleQuoteCard *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::JustWatch, _aqPtr);
        // ui->quotesLayout->addWidget(sqc);
        emplaceQuoteToGroupFrame(sqc, sqm->owner());
    }

    delete quotes;
}

void SearchTab::sendSearchQuoteRequest()
{
    if (ui->quoteLE->text().trimmed() == "")
        return;
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
    _groups.clear();
}

void SearchTab::emplaceQuoteToGroupFrame(SingleQuoteCard* quote, const QString& group)
{
    if (_groups.contains(group))
    {
        _groupLayouts[group]->addWidget(quote);
        return;
    }
    _groups.append(group);
    QGroupBox* gb = new QGroupBox(tr(group.toUtf8().constData()));
    ui->quotesLayout->addWidget(gb);

    QVBoxLayout *vbox = new QVBoxLayout();
    gb->setLayout(vbox);
    _groupLayouts.insert(group, vbox);

    vbox->addWidget(quote);
}

void SearchTab::addChosenAttrs(const QString& attribute)
{
    if (_chosenAttrs.contains(attribute))
        return;
    _chosenAttrs.append(attribute);
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    auto label = new ClickableLabel<SearchTab>(QStringLiteral("\u2A09 ") + attribute, &SearchTab::removeAttr, this);
    label->setFont(QFont("Segoe UI", 10));
    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label->setParent(frame);

    frame->setFixedHeight(label->sizeHint().height() + 10);
    frame->setFixedWidth(label->sizeHint().width() + 7);

    ui->chosenAttrsLayout->addWidget(frame);
    _attrsLabels.append(label);
}

void SearchTab::removeAttr(ClickableLabel<SearchTab>* caller)
{
    QString s = caller->text();
    uncheckedAttr(s.mid(2));
    emit sendToUncheckMenu(s.mid(2));
}

void SearchTab::uncheckedAttr(const QString& attr)
{
    _chosenAttrs.removeAll(attr);
    for (auto& item: _attrsLabels)
    {
        if (item->text().mid(2) == attr)
        {
            item->parent()->deleteLater();
            _attrsLabels.removeAll(item);
            break;
        }
    }
}
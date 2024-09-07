#include "authedquotes.h"
#include "addownerdialog.h"
#include "dialogupdatequote.h"
#include "inputowner.h"
#include "qmessagebox.h"
#include "quoteinput.h"
#include "singlequotecard.h"
#include "ui_authedquotes.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QScrollBar>
#include "registerform.h"

AuthedQuotes::AuthedQuotes(std::shared_ptr<QuotesApiRepresenter> apiPresenter, QString username, QWidget *parent)
    : QTabWidget(parent)
    , ui(new Ui::AuthedQuotes)
    , _quotesApi(apiPresenter)
{
    ui->setupUi(this);

    ui->homepageScrollArea->horizontalScrollBar()->setEnabled(false);
    ui->quotesScrollArea->horizontalScrollBar()->setEnabled(false);

    favouriteQuotesLabel = new ClickableLabel("Favourite ones", &AuthedQuotes::updateFavsQuotesRequest, this);
    addedQuotesLabel = new ClickableLabel("Added ones", &AuthedQuotes::updateAddedQuotesRequest, this);
    groupInfoLabel = new ClickableLabel(QString::fromUtf16(u"\U0001F6C8"), &AuthedQuotes::getGroupInfo, this);

    QFont font = favouriteQuotesLabel->font();
    font.setBold(true);
    favouriteQuotesLabel->setFont(font);

    ui->quotesLabelsLayout->addWidget(favouriteQuotesLabel);
    ui->quotesLabelsLayout->addWidget(addedQuotesLabel);
    ui->horizontalLayout_3->addWidget(groupInfoLabel);
    //connect(_apiThread, &QThread::finished, _apiThread, &QThread::deleteLater);

    ui->usernameLabel->setText(username);

    QObject::connect(ui->logoutBtn, &QPushButton::clicked, (MainWindow*)parent, &MainWindow::startForm);


    QObject::connect(ui->addQuoteBtn, &QPushButton::clicked, this, &AuthedQuotes::onAddQuoteClicked);
    QObject::connect(ui->addOwnerBtn, &QPushButton::clicked, this, &AuthedQuotes::onAddOwnerClicked);

    // send requests to API to get core tables
    QObject::connect(this, &AuthedQuotes::sendGetCoreTableRequest, _quotesApi.get(), &QuotesApiRepresenter::getCoreTable);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendOwnersValues, this, &AuthedQuotes::setOwners);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendAttrsValues, this, &AuthedQuotes::setAttrs);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendFeaturesValues, this, &AuthedQuotes::setFeatures);

    // get quotes by owner and favourite ones
    QObject::connect(ui->ownersComdoBox, &QComboBox::currentTextChanged, _quotesApi.get(), &QuotesApiRepresenter::getQuoteCards);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendQuoteOwnerCards, this, &AuthedQuotes::onOwnersComdoBoxCurrentTextChanged);
    
    QObject::connect(this, &AuthedQuotes::sendUpdateFavsQuotesRequest, _quotesApi.get(), &QuotesApiRepresenter::getFavouriteCards);
    
    QObject::connect(ui->updateFavs, &QPushButton::clicked, this, &AuthedQuotes::onUpdatePageClicked);

    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendQuoteFavouriteCards, this, &AuthedQuotes::onUpdateFavsClicked);

    // get added quotes
    QObject::connect(this, &AuthedQuotes::sendUpdateAddedQuotesRequest, _quotesApi.get(), &QuotesApiRepresenter::getPossessedCards);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendQuotePossessedCards, this, &AuthedQuotes::onUpdateAddedClicked);

    // add favourite quote
    QObject::connect(this, &AuthedQuotes::addFavouriteQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::addFavorite);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendFavouriteQuoteStatus, this, &AuthedQuotes::favouriteQuoteAction);

    // remove favourite quote
    QObject::connect(this, &AuthedQuotes::removeFavouriteQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::removeFavorite);

    // add owner
    QObject::connect(this, &AuthedQuotes::sendAddOwnerRequest, _quotesApi.get(), &QuotesApiRepresenter::addOwner);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendIfOwnerAddedResponse, this, &AuthedQuotes::addOwnerResponse);

    // add quote
    QObject::connect(this, &AuthedQuotes::sendAddQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::addQuote);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendIfQuoteAddedResponse, this, &AuthedQuotes::addQuoteResponse);

    // update quote
    QObject::connect(this, &AuthedQuotes::sendUpdateQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::updateQuote);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendIfQuoteUpdatedResponse, this, &AuthedQuotes::updateQuoteResponse);

    // delete quote
    QObject::connect(this, &AuthedQuotes::sendDeleteQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::deleteQuote);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendIfQuoteDeletedResponse, this, &AuthedQuotes::deleteQuoteResponse);

    // previous and next buttons
    QObject::connect(ui->previousGroupBtn, &QPushButton::clicked, this, &AuthedQuotes::onPreviousBtnClicked);
    QObject::connect(ui->nextGroupBtn, &QPushButton::clicked, this, &AuthedQuotes::onNextBtnClickedClick);

    QObject::connect(ui->getRandomQuoteBtn, &QPushButton::clicked, this, &AuthedQuotes::onGetRandomQuoteClicked);
    QObject::connect(this, &AuthedQuotes::getRandomQuoteRequest, _quotesApi.get(), &QuotesApiRepresenter::getRandomQuote);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendRandomQuote, this, &AuthedQuotes::randomQuoteResponse);

    QObject::connect(this, &AuthedQuotes::sendGetGroupDescriptionRequest, _quotesApi.get(), &QuotesApiRepresenter::getGroupDescription);
    QObject::connect(_quotesApi.get(), &QuotesApiRepresenter::sendGroupDescription, this, &AuthedQuotes::groupDescriptionResponse);

    if (username == "John Doe")
    {
        for(auto& w: ui->userTab->findChildren<QWidget*>())
        {
            w->setEnabled(false);
        }
        ui->logoutBtn->setEnabled(true);
        //this->removeTab(0);
        this->setCurrentIndex(1);
    } else emit sendUpdateFavsQuotesRequest();

    getCoreTable(QuotesApi::CoreTables::attrs);
    getCoreTable(QuotesApi::CoreTables::features);
    getCoreTable(QuotesApi::CoreTables::owners);
}

AuthedQuotes::~AuthedQuotes()
{
    qDebug() << "~AuthedQuotes";
    //_apiThread->exit();
    //_apiThread->deleteLater();
    delete ui;
    delete _owners;
    delete _attrs;
    delete _features;
    delete favouriteQuotesLabel;
    delete addedQuotesLabel;
}

void AuthedQuotes::getCoreTable(QuotesApi::CoreTables what)
{
    emit sendGetCoreTableRequest(what);
}

void AuthedQuotes::setOwners(QStringList *owners)
{
    _owners = owners;
    QString owner;
    foreach(owner, *owners)
    {
        ui->ownersComdoBox->addItem(owner);
    }
}

void AuthedQuotes::setAttrs(QStringList *attrs)
{
    _attrs = attrs;
}

void AuthedQuotes::setFeatures(QStringList *features)
{
    _features = features;
    searchTab = std::make_unique<SearchTab>(_quotesApi, this);
    ui->searchTab->layout()->addWidget(searchTab.get());
}

void AuthedQuotes::freeQuotesLayout(QLayout* l)
{
    QLayoutItem* item;
    while ((item = l->takeAt(0)) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void AuthedQuotes::onOwnersComdoBoxCurrentTextChanged(QList<std::shared_ptr<SingleQuoteModel>>* quotes)
{
    freeQuotesLayout(ui->quotesLayout);

    foreach(const std::shared_ptr<SingleQuoteModel> &sqm, *quotes)
    {
        QWidget *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::JustWatch, this);
        ui->quotesLayout->addWidget(sqc);
    }

    delete quotes;
}

void AuthedQuotes::favouriteQuoteAction(bool success, const QString& message)
{
    if(!success)
    {
        QMessageBox msg(this);

        msg.setText("Error occupied while was " + message + " quote");
        msg.exec();
        return;
    }
    onUpdatePageClicked();
}

void AuthedQuotes::favouriteQuoteFromSingleCard(SingleQuoteModel *model)
{
    emit addFavouriteQuoteRequest(model);
}

void AuthedQuotes::aboutToDeleteQuoteFromSingleCard(SingleQuoteModel* model)
{
    QMessageBox msgBox(this);
    msgBox.setText("Do you wanna delete that card?");

    msgBox.setWindowTitle("title");
    // msgBox.setText("Question");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) emit sendDeleteQuoteRequest(model);
}

void AuthedQuotes::onUpdateFavsClicked(QList<std::shared_ptr<SingleQuoteModel>>* quotes)
{

    freeQuotesLayout(ui->favQuotesLayout);

    foreach(const std::shared_ptr<SingleQuoteModel> &sqm, *quotes)
    {
        QWidget *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::Favourite, this);
        ui->favQuotesLayout->addWidget(sqc);
    }

    delete quotes;
}

void AuthedQuotes::updateAddedQuotesRequest(ClickableLabel<AuthedQuotes>*)
{
    changeLabelsState(addedQuotesLabel);
    emit sendUpdateAddedQuotesRequest();
}

void AuthedQuotes::changeLabelsState(ClickableLabel<AuthedQuotes>* caller)
{
    if (caller->font().bold())
        return;

    QFont font = favouriteQuotesLabel->font();
    font.setBold(!font.bold());
    favouriteQuotesLabel->setFont(font);

    QFont font1 = addedQuotesLabel->font();
    font1.setBold(!font1.bold());
    addedQuotesLabel->setFont(font1);
}

void AuthedQuotes::updateFavsQuotesRequest(ClickableLabel<AuthedQuotes>*)
{
    changeLabelsState(favouriteQuotesLabel);
    // emit ui->updateFavs->clicked();
    emit sendUpdateFavsQuotesRequest();
}

void AuthedQuotes::onUpdateAddedClicked(QList<std::shared_ptr<SingleQuoteModel>>* quotes)
{

    freeQuotesLayout(ui->favQuotesLayout);

    foreach(const std::shared_ptr<SingleQuoteModel> &sqm, *quotes)
    {
        QWidget *sqc = new SingleQuoteCard(sqm, SingleQuoteCard::QuoteMode::JustWatch, this);
        ui->favQuotesLayout->addWidget(sqc);
    }

    delete quotes;
}

void AuthedQuotes::updateQuoteFromSingleCard(SingleQuoteModel* model)
{
    QuoteInput* inputForm = new QuoteInput(_owners, _attrs, _features, model, this);

    DialogUpdateQuote d{inputForm, this};
    d.exec();
}

void AuthedQuotes::onAddQuoteClicked()
{
    QuoteInput* inputForm = new QuoteInput(_owners, _attrs, _features, this);

    DialogUpdateQuote d{inputForm, this};
    d.exec();
}

void AuthedQuotes::onAddOwnerClicked()
{
    InputOwner* inputForm = new InputOwner(this);

    AddOwnerDialog d{inputForm, this};
    d.exec();
}

void AuthedQuotes::quoteAddAccepted(SingleQuoteModel* model)
{
    emit sendAddQuoteRequest(model);
}

void AuthedQuotes::quoteUpdateAccepted(SingleQuoteModel* data, SingleQuoteModel* original)
{
    emit sendUpdateQuoteRequest(data, original);
}

void AuthedQuotes::addOwnerAccepted(std::pair<QString, QString> data)
{
    emit sendAddOwnerRequest(data);
}

void AuthedQuotes::createResponseWindow(QString errorMessage, bool success)
{
    QString text = success? "Success" : errorMessage;
    QMessageBox msg(this);

    msg.setText(text);
    msg.exec();
}

void AuthedQuotes::addQuoteResponse(bool success)
{
    createResponseWindow("Error occured while was adding quote =(", success);
    if (success)
        onUpdatePageClicked();
}

void AuthedQuotes::updateQuoteResponse(QString success)
{
    onUpdatePageClicked();
    QString text = success;
    QMessageBox msg(this);
    msg.setText(text);
    msg.exec();
}

void AuthedQuotes::deleteQuoteResponse(bool success)
{
    createResponseWindow("Error occured while was deleting quote =(", success);
    if (success)
        onUpdatePageClicked();
}

void AuthedQuotes::addOwnerResponse(bool success)
{
    createResponseWindow("Error occured while was adding group =(", success);
    if (success)
        getCoreTable(QuotesApi::CoreTables::owners);
}

void AuthedQuotes::removeFavouriteQuoteFromSingleCard(SingleQuoteModel* model)
{
    emit removeFavouriteQuoteRequest(model);
    // updateFavsQuotesRequest();
}

void AuthedQuotes::onUpdatePageClicked()
{
    // getCoreTable(QuotesApi::CoreTables::features);
    // getCoreTable(QuotesApi::CoreTables::attrs);
    // getCoreTable(QuotesApi::CoreTables::owners);
    if (favouriteQuotesLabel->font().bold())
        updateFavsQuotesRequest(nullptr);
    else updateAddedQuotesRequest(nullptr);
    
    emit ui->ownersComdoBox->currentTextChanged(ui->ownersComdoBox->currentText());
}

void AuthedQuotes::onPreviousBtnClicked()
{
    int currentIndex = ui->ownersComdoBox->currentIndex();
    if (currentIndex == 0)
        ui->ownersComdoBox->setCurrentIndex(ui->ownersComdoBox->count() - 1);
    else ui->ownersComdoBox->setCurrentIndex(currentIndex - 1);
}

void AuthedQuotes::onNextBtnClickedClick()
{
    int currentIndex = ui->ownersComdoBox->currentIndex();
    if (currentIndex == ui->ownersComdoBox->count() - 1)
        ui->ownersComdoBox->setCurrentIndex(0);
    else ui->ownersComdoBox->setCurrentIndex(currentIndex + 1);
}

void AuthedQuotes::onGetRandomQuoteClicked()
{
    emit getRandomQuoteRequest();
}

void AuthedQuotes::randomQuoteResponse(SingleQuoteModel* model)
{
    SingleQuoteCard *sqm = new SingleQuoteCard(std::make_shared<SingleQuoteModel>(*model), SingleQuoteCard::QuoteMode::JustWatch, this);
    sqm->setWindowFlags(Qt::Window);
    sqm->show();

    delete model;
}

void AuthedQuotes::onChangePasswordClicked()
{
    // RegisterForm *rw = new RegisterForm(this);
    // rw->setWindowFlags(Qt::Dialog);
    // rw->show();
}

void AuthedQuotes::getGroupInfo(ClickableLabel<AuthedQuotes>* caller)
{
    emit sendGetGroupDescriptionRequest(ui->ownersComdoBox->currentText());
}

void AuthedQuotes::groupDescriptionResponse(const QString& group)
{
    QLabel *label = new QLabel(group, this);
    label->setWindowFlags(Qt::Sheet);
    label->setFont(QFont("Segou UI", 20));
    label->setWindowTitle(ui->ownersComdoBox->currentText());
    label->show();
}
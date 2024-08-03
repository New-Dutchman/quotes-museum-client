#include "dialogupdatequote.h"
#include "quoteinput.h"
#include "ui_dialogupdatequote.h"

DialogUpdateQuote::DialogUpdateQuote(QuoteInput* w, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogUpdateQuote)
{
    inputForm = w;
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, w);

    QObject::connect(this, &DialogUpdateQuote::accepted, inputForm, &QuoteInput::getReadyToSend);
}

DialogUpdateQuote::~DialogUpdateQuote()
{
    delete ui;
    delete inputForm;
}

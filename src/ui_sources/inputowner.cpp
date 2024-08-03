#include "inputowner.h"
#include "authedquotes.h"
#include "ui_inputowner.h"

InputOwner::InputOwner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputOwner)
{
    ui->setupUi(this);

    QObject::connect(this, &InputOwner::sentOwner, (AuthedQuotes*)parent, &AuthedQuotes::addOwnerAccepted);
}

InputOwner::~InputOwner()
{
    delete ui;
}


void InputOwner::getReadyToSend()
{
    std::pair<QString, QString> data{ui->ownerLE->text(), ui->descLE->text()};
    emit sentOwner(data);
}

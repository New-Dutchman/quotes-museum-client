#include "addownerdialog.h"
#include "ui_addownerdialog.h"

AddOwnerDialog::AddOwnerDialog(InputOwner *w, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddOwnerDialog)
{
    inputForm = w;
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, w);

    QObject::connect(this, &AddOwnerDialog::accepted, inputForm, &InputOwner::getReadyToSend);
}

AddOwnerDialog::~AddOwnerDialog()
{
    delete ui;
    delete inputForm;
}

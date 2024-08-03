#include "quoteinput.h"
#include "authedquotes.h"
#include "ui_quoteinput.h"

QuoteInput::QuoteInput(QList<QString>* owners, QList<QString>* attrs, QList<QString>* features, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuoteInput)
{
    ui->setupUi(this);

    _mode = AddNew;

    _id = -1;
    _cites = 0;
    QString s;
    foreach(s, *owners)
    {
        ui->ownerCB->addItem(s);
    }

    foreach(s, *attrs)
    {
        QWidget *cb = new QCheckBox(s, this);
        QWidget *le = new QLineEdit(this);

        //cb->setObjectName(s);
        //le->setObjectName(s + "_le");

        cb->setFont(QFont("Segoe", 14));
        le->setFont(QFont("Segoe", 14));
        le->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

        QHBoxLayout *l = new QHBoxLayout(this);
        l->addWidget(cb);
        l->addWidget(le);

        l->setObjectName(s + "_big_le");

        ui->attrLayout->addLayout(l);

    }

    foreach(s, *features)
    {
        QWidget *cb1 = new QCheckBox(s, this);

        ui->featuresLayout->addWidget(cb1);
    }

    QObject::connect(this, &QuoteInput::sendCreatedQuote, (AuthedQuotes*)parent, &AuthedQuotes::quoteAddAccepted);
    QObject::connect(this, &QuoteInput::sendUpdatedQuote, (AuthedQuotes*)parent, &AuthedQuotes::quoteUpdateAccepted);

}

QuoteInput::QuoteInput(QList<QString>* owners, QList<QString>* attrs, QList<QString>* features, SingleQuoteModel* model, QWidget *parent) :
    QuoteInput(owners, attrs, features, parent)
{
    _mode = UpdateExisting;
    _model = model;
    _id = model->id();
    _cites = model->cites();

    ui->ownerCB->setCurrentText(model->owner());
    ui->quoteLE->setText(model->quote());
    ui->obscenceCheck->setChecked(model->is_obscene());

    for (int i = 0; i < ui->featuresLayout->count(); i++)
    {
        QCheckBox* w = (QCheckBox*)ui->featuresLayout->itemAt(i)->widget();
        if (model->features().contains(w->text())) w->setChecked(true);
    }

    qDebug() << ui->attrLayout->count();
    for (int lay = 0; lay < ui->attrLayout->count(); lay++)
    {
        QCheckBox* cb = (QCheckBox*)ui->attrLayout->itemAt(lay)->layout()->itemAt(0)->widget();
        QLineEdit* le = (QLineEdit*)ui->attrLayout->itemAt(lay)->layout()->itemAt(1)->widget();

        if (model->attrs().contains(cb->text()))
        {
            cb->setChecked(true);
            le->setText(model->attrs().value(cb->text()));
        }
    }
}

SingleQuoteModel* QuoteInput::makeQuote()
{
    QList<QString>* f = new QList<QString>;
    QMap<QString, QString>* a = new QMap<QString, QString>;

    for (int i = 0; i < ui->featuresLayout->count(); i++)
    {
        QCheckBox* w = (QCheckBox*)ui->featuresLayout->itemAt(i)->widget();
        if (w->isChecked()) f->append(w->text());
    }

    for (int i = 0; i < ui->attrLayout->count(); i++)
    {
        QCheckBox* cb = (QCheckBox*)ui->attrLayout->itemAt(i)->layout()->itemAt(0)->widget();
        QLineEdit* le = (QLineEdit*)ui->attrLayout->itemAt(i)->layout()->itemAt(1)->widget();

        if (cb->isChecked()) a->insert(cb->text(), le->text());
    }

    SingleQuoteModel* data = new SingleQuoteModel(_id,
                                                  ui->quoteLE->text(),
                                                  ui->obscenceCheck->isChecked(),
                                                  _cites,
                                                  f,
                                                  a,
                                                  ui->ownerCB->currentText());

    return data;
}

void QuoteInput::getReadyToSend()
{
    qDebug() << _mode;
    auto d = makeQuote();
    switch (_mode)
    {
    case AddNew: emit sendCreatedQuote(d); break;
    case UpdateExisting: emit sendUpdatedQuote(d, _model); break;
    }
}

QuoteInput::~QuoteInput()
{
    delete ui;
}

#include "quoteinput.h"
#include "authedquotes.h"
#include "ui_quoteinput.h"

QuoteInput::QuoteInput(QList<QString>* owners, QList<QString>* attrs, QList<QString>* features, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuoteInput)
{
    ui->setupUi(this);

    _mode = AddNew;

    QString styleGB = "QGroupBox {"
                        "border: 2px solid black;"  // Толщина и цвет обводки
                        "border-radius: 5px;"       // Скругленные углы (по желанию)
                        "margin-top: 1.25em;"        // Отступ сверху для заголовка
                        "}"
                        "QGroupBox::title {"
                        "subcontrol-origin: margin;" 
                        "subcontrol-position: top left;"  // Заголовок сверху слева
                        "padding: 0 3px;"                // Отступ вокруг заголовка
                        "color: black;"                  // Цвет текста заголовка
                        "}";

    ui->attrsGB->setStyleSheet(styleGB);

    ui->featuresGB->setStyleSheet(styleGB);

    _id = -1;
    _cites = 0;
    QString s;
    foreach(s, *owners)
    {
        ui->ownerCB->addItem(s);
    }

    foreach(s, *attrs)
    {
        QWidget *cb = new QLabel(s, this);
        QWidget *le = new QLineEdit(this);

        //cb->setObjectName(s);
        //le->setObjectName(s + "_le");

        cb->setFont(QFont("Segoe", 14));
        le->setFont(QFont("Segoe", 14));
        le->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

        ui->formLayout->addRow(cb, le);
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

    for (int row = 0; row < ui->formLayout->rowCount(); row++)
    {
        QLabel* cb = (QLabel*)ui->formLayout->itemAt(row, QFormLayout::LabelRole)->widget();
        QLineEdit* le = (QLineEdit*)ui->formLayout->itemAt(row, QFormLayout::FieldRole)->widget();

        if (model->attrs().contains(cb->text()))
        {
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

    for (int row = 0; row < ui->formLayout->rowCount(); row++)
    {
        QLabel* cb = (QLabel*)ui->formLayout->itemAt(row, QFormLayout::LabelRole)->widget();
        QLineEdit* le = (QLineEdit*)ui->formLayout->itemAt(row, QFormLayout::FieldRole)->widget();
        
        QString label = le->text().trimmed();
        if (le->text().trimmed() != "") a->insert(cb->text(), le->text());
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
        case AddNew: 
            emit sendCreatedQuote(d); 
            break;

        case UpdateExisting: 
            emit sendUpdatedQuote(d, _model); 
            break;
    }
}

QuoteInput::~QuoteInput()
{
    delete ui;
}

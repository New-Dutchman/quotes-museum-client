#ifndef QUOTEINPUT_H
#define QUOTEINPUT_H

#include "singlequotemodel.h"
#include <QWidget>

namespace Ui {
class QuoteInput;
}

class QuoteInput : public QWidget
{
    Q_OBJECT

public:
    explicit QuoteInput(QList<QString>* owners, QList<QString>* attrs, QList<QString>* features, QWidget *parent = nullptr);
    QuoteInput(QList<QString>* owners, QList<QString>* attrs, QList<QString>* features, SingleQuoteModel* model, QWidget *parent);
    ~QuoteInput();

    SingleQuoteModel* makeQuote();

    enum QuoteMode{AddNew, UpdateExisting};

public slots:
    void getReadyToSend();

public: signals:
    void sendCreatedQuote(SingleQuoteModel*);
    void sendUpdatedQuote(SingleQuoteModel*, SingleQuoteModel*);

private:
    Ui::QuoteInput *ui;
    SingleQuoteModel* _model;

    QuoteMode _mode;

    int _id;
    int _cites;
};

#endif // QUOTEINPUT_H

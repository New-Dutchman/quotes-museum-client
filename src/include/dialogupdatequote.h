#ifndef DIALOGUPDATEQUOTE_H
#define DIALOGUPDATEQUOTE_H

#include "quoteinput.h"
#include <QDialog>

namespace Ui {
class DialogUpdateQuote;
}

class DialogUpdateQuote : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpdateQuote(QuoteInput* w, QWidget *parent);
    ~DialogUpdateQuote();

private:
    Ui::DialogUpdateQuote *ui;
    QuoteInput *inputForm;
};

#endif // DIALOGUPDATEQUOTE_H

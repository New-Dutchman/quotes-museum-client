#ifndef ADDOWNERDIALOG_H
#define ADDOWNERDIALOG_H

#include "inputowner.h"
#include <QDialog>

namespace Ui {
class AddOwnerDialog;
}

class AddOwnerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddOwnerDialog(InputOwner* w, QWidget *parent = nullptr);
    ~AddOwnerDialog();

private:
    Ui::AddOwnerDialog *ui;
    InputOwner *inputForm;
};

#endif // ADDOWNERDIALOG_H

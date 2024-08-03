#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = nullptr);
    ~RegisterForm();


private:
    Ui::RegisterForm *ui;

signals:
    void sendUser(QString, QString);

private slots:
    void cancel();
    void createUser();
};

#endif // REGISTERFORM_H

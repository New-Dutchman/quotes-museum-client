#ifndef STARTFORM_H
#define STARTFORM_H

#include "connection.h"
#include "user.h"
#include <QWidget>

namespace Ui {
class StartForm;
}

class StartForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartForm(std::shared_ptr<User> user, std::shared_ptr<Connection> conn, QWidget *parent = nullptr);
    ~StartForm();

    // void setText();

private:
    Ui::StartForm *ui;
    std::shared_ptr<Connection> _conn;
    std::shared_ptr<User> _user;

private slots:
    void onLoginClick();
    void defaultUserChecked();
    void defaultConnectionChecked();

signals:
    void loginRequest();
};

#endif // STARTFORM_H

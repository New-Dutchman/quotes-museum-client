#ifndef STARTFORM_H
#define STARTFORM_H

#include "connection.h"
#include "user.h"
#include <QWidget>
#include <QCompleter>

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
    QMap<QString, QString> _users;
    QString _userDataFile = "userData";

    QCompleter *completer;

    void readUsers();

private slots:
    void onLoginClick();
    void defaultUserChecked();
    void defaultConnectionChecked();
    void rememberUser();
    void completeUserPassword(const QString& username);

    void onRegisterClick();

signals:
    void loginRequest();
    void registerRequest();
};

#endif // STARTFORM_H

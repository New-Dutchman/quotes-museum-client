#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connection.h"
#include "user.h"
#include <QMainWindow>
#include <QLayout>
#include "quotesapirepresenter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startForm();
    void authorization();
    void createNewUser(QString, QString);
    void registerForm();

    void login();
    void forbid();
    void checkAddedUser(QString);

signals:
    void pleaseAuthenticateRequest(std::shared_ptr<User> user, std::shared_ptr<Connection> conn);
    void pleaseRegisterNewUserRequest(std::shared_ptr<User> user, std::shared_ptr<Connection> conn);
    void rememberUser();

private:
    Ui::MainWindow *ui;
    QWidget *widget;

    std::shared_ptr<QuotesApiRepresenter> _apiRepresenter;

    std::shared_ptr<Connection> _conn;
    std::shared_ptr<User> _user;
};
#endif // MAINWINDOW_H

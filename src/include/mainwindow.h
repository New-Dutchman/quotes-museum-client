#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connection.h"
#include "user.h"
#include <QMainWindow>
#include <QLayout>

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

    //void onLoginClick();
public slots:
    void startForm();
    void authorization();
    void createNewUser(QString, QString);
    void registerForm();



private:
    Ui::MainWindow *ui;
    QWidget *widget;
    //QLayout *layout;
    //QStackedWidget *stack;

    std::shared_ptr<Connection> _conn;
    std::shared_ptr<User> _user;

    void authed();
};
#endif // MAINWINDOW_H

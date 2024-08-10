#include "mainwindow.h"
#include "authedquotes.h"
#include "qmessagebox.h"
#include "registerform.h"
#include "startform.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _conn = std::make_shared<Connection>();
    _user = std::make_shared<User>();
    widget = new StartForm(_user, _conn, this);
    _apiRepresenter = std::make_shared<QuotesApiRepresenter>();
    ui->verticalLayout->addWidget(widget);

    QObject::connect(this, &MainWindow::pleaseAuthenticateRequest, _apiRepresenter.get(), &QuotesApiRepresenter::tryAuthenticate);
    QObject::connect(_apiRepresenter.get(), &QuotesApiRepresenter::forbidden, this, &MainWindow::forbid);
    QObject::connect(_apiRepresenter.get(), &QuotesApiRepresenter::authenticated, this, &MainWindow::login);

    QObject::connect(this, &MainWindow::pleaseRegisterNewUserRequest, _apiRepresenter.get(), &QuotesApiRepresenter::addUser);
    QObject::connect(_apiRepresenter.get(), &QuotesApiRepresenter::addedUserAnswer, this, &MainWindow::checkAddedUser);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete(widget);
}

void MainWindow::startForm()
{
    ui-> verticalLayout->removeWidget(widget);
    delete(widget);
    widget = new StartForm(_user, _conn, this);
    ui->verticalLayout->addWidget(widget);
}

void MainWindow::authorization()
{
    qDebug() << "MainWindow::authorization";
    emit pleaseAuthenticateRequest(_user, _conn);
}

void MainWindow::forbid()
{
    QMessageBox msgBox;
    QString text = "Login error :( ";
    msgBox.setText(text);
    qDebug() << text;
    msgBox.exec();
}

void MainWindow::login()
{   
    emit rememberUser();
    qDebug() << "MainWindow::authed";
    ui-> verticalLayout->removeWidget(widget);
    widget->deleteLater();
    widget = new AuthedQuotes(_apiRepresenter, _user->username(), this);
    ui->verticalLayout->addWidget(widget);
}

void MainWindow::registerForm()
{
    QWidget *rw = new RegisterForm(this);
    rw->setWindowFlags(Qt::Dialog);
    rw->show();
}

void MainWindow::createNewUser(QString name, QString password)
{
    User newUser = User(name, password);

    emit pleaseRegisterNewUserRequest(&newUser);

}

void MainWindow::checkAddedUser(QString answer)
{
    QMessageBox msgBox;
    QString text = answer == "Success"? "Good!" : "Failure occured";
    msgBox.setText(text);
    msgBox.exec();
}

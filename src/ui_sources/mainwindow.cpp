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

    ui->verticalLayout->addWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete(widget);
}

/*
MainWindow* MainWindow::getInstance()
{
    return instance;
}
*/

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
    QuotesApiRepresenter* representer = new QuotesApiRepresenter(std::make_shared<QuotesApi>(_conn, _user));

    bool access = representer->authed();

    delete representer;

    if (access)
    {
        authed();
    } else {
        QMessageBox msgBox;
        QString text = "Login error :( "; // + db->lastErrorText();
        msgBox.setText(text);
        qDebug() << text;
        msgBox.exec();
    }
}

void MainWindow::authed()
{
    qDebug() << "MainWindow::authed";
    ui-> verticalLayout->removeWidget(widget);
    delete(widget);
    widget = new AuthedQuotes(_conn, _user, this);
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

    QuotesApiRepresenter* representer = new QuotesApiRepresenter(std::make_shared<QuotesApi>(_conn, _user));

    if (representer->addUser(&newUser) == "Success")
    {
        QMessageBox msgBox;
        QString text = "Good!";
        msgBox.setText(text);
        msgBox.exec();
    }
    else{
        QMessageBox msgBox;
        QString text = "Failure occured";
        msgBox.setText(text);
        qDebug() << text;
        msgBox.exec();
    }

    delete representer;
}







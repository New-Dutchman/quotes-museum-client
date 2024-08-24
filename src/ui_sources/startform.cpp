#include "startform.h"
#include "ui_startform.h"
#include "mainwindow.h"
#include <QFile>
#include <Qmap>
#include <QOverload>

StartForm::StartForm(std::shared_ptr<User> user, std::shared_ptr<Connection> conn, QWidget *parent)
    : QWidget(parent), ui(new Ui::StartForm), _conn(conn), _user(user)
{
    ui->setupUi(this);
    ui->formLayout->setAlignment(ui->defaultUserCheckBox, Qt::AlignRight);

    ui->defaultUserCheckBox->setLayoutDirection(Qt::RightToLeft);
    ui->defaultServerCheckBox->setLayoutDirection(Qt::RightToLeft);
    readUsers();

    completer = new QCompleter(_users.keys(), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->usernameLE->setCompleter(completer);

    QObject::connect(ui->loginBtn, &QPushButton::clicked, this, &StartForm::onLoginClick);
    QObject::connect(this, &StartForm::loginRequest, (MainWindow*)this->parent(), &MainWindow::authorization);
    QObject::connect((MainWindow*)this->parent(), &MainWindow::rememberUser, this, &StartForm::rememberUser);

    QObject::connect(completer, QOverload<const QString &>::of(&QCompleter::activated), this, &StartForm::completeUserPassword);

    QObject::connect(ui->registerBtn, &QPushButton::clicked, this, &StartForm::onRegisterClick);
    QObject::connect(this, &StartForm::registerRequest, (MainWindow*)this->parent(), &MainWindow::registerForm);

    QObject::connect(ui->defaultUserCheckBox, &QAbstractButton::clicked, this, &StartForm::defaultUserChecked);
    QObject::connect(ui->defaultServerCheckBox, &QAbstractButton::clicked, this, &StartForm::defaultConnectionChecked);

}

StartForm::~StartForm()
{
    delete completer;
    delete ui;
    qDebug() << "StartForm widget destructor";
}

void StartForm::onLoginClick()
{
    if (!ui->defaultUserCheckBox->isChecked())
    {
        _user->username(ui->usernameLE->text(), _user->passwd());
        _user->passwd(ui->passwordLE->text(), _user->passwd());
    }

    _conn->port(ui->portLE->text().toInt());
    _conn->server(ui->hostnameLE->text());

    emit loginRequest();
}

void StartForm::defaultUserChecked()
{
    if (ui->defaultUserCheckBox->isChecked())
    {
        ui->usernameLE->setDisabled(true);
        ui->passwordLE->setDisabled(true);
    }
    else
    {
        ui->usernameLE->setDisabled(false);
        ui->passwordLE->setDisabled(false);
    }
}

void StartForm::defaultConnectionChecked()
{
    if (ui->defaultServerCheckBox->isChecked())
    {
        ui->hostnameLE->setText("localhost");
        ui->portLE->setText("8080");
        ui->hostnameLE->setDisabled(true);
        ui->portLE->setDisabled(true);
    }
    else
    {
        ui->hostnameLE->setDisabled(false);
        ui->portLE->setDisabled(false);
    }
}

void StartForm::onRegisterClick()
{
    if (!ui->defaultUserCheckBox->isChecked())
    {
        _user->username(ui->usernameLE->text(), _user->passwd());
        _user->passwd(ui->passwordLE->text(), _user->passwd());
    }

    _conn->port(ui->portLE->text().toInt());
    _conn->server(ui->hostnameLE->text());

    emit registerRequest();
}

void StartForm::readUsers()
{
    QFile userData(_userDataFile);

    if (userData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream input(&userData);
        QStringList users;
        while (!input.atEnd())
        {
            users = input.readLine().split("\t");
            assert(users.size() == 2);
            _users.insert(users[0], users[1]);
        }
    }
}

void StartForm::rememberUser()
{
    if (!ui->rememberMeCheckBox->isChecked() ||
         ui->defaultUserCheckBox->isChecked() ||
         _users.contains(_user->username()))
    {
        return;
    }
    QFile file(_userDataFile);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << _user->username() << '\t' << _user->passwd() << '\n';
        file.close();
    }
}

void StartForm::completeUserPassword(const QString& username)
{
    ui->passwordLE->setText(_users[username]);
}
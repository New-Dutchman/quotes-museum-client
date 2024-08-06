#include "startform.h"
#include "ui_startform.h"
#include "mainwindow.h"

StartForm::StartForm(std::shared_ptr<User> user, std::shared_ptr<Connection> conn, QWidget *parent)
    : QWidget(parent), ui(new Ui::StartForm), _conn(conn), _user(user)
{
    ui->setupUi(this);
    ui->formLayout->setAlignment(ui->defaultUserCheckBox, Qt::AlignRight);

    ui->defaultUserCheckBox->setLayoutDirection(Qt::RightToLeft);
    ui->defaultServerCheckBox->setLayoutDirection(Qt::RightToLeft);

    QObject::connect(ui->loginBtn, &QPushButton::clicked, this, &StartForm::onLoginClick);
    QObject::connect(this, &StartForm::loginRequest, (MainWindow*)this->parent(), &MainWindow::authorization);

    QObject::connect(ui->registerBtn, &QPushButton::clicked, (MainWindow*)this->parent(), &MainWindow::registerForm);

    QObject::connect(ui->defaultUserCheckBox, &QAbstractButton::clicked, this, &StartForm::defaultUserChecked);
    QObject::connect(ui->defaultServerCheckBox, &QAbstractButton::clicked, this, &StartForm::defaultConnectionChecked);

}

StartForm::~StartForm()
{
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

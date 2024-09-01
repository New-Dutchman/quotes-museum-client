#include "registerform.h"
#include "mainwindow.h"
#include "qmessagebox.h"
#include "ui_registerform.h"

RegisterForm::RegisterForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterForm)
{
    qDebug() << "RegisterForm constructor";
    ui->setupUi(this);

    QObject::connect(ui->cancelBtn, &QPushButton::clicked, this, &RegisterForm::cancel);
    QObject::connect(ui->createBtn, &QPushButton::clicked, this, &RegisterForm::createUser);
}

RegisterForm::~RegisterForm()
{
    qDebug() << "RegisterForm destructor";
    delete ui;
}

void RegisterForm::cancel()
{
    this->deleteLater();
}

void RegisterForm::createUser()
{
    QString name = ui->usernameLE->text();
    QString password = ui->passwdLE->text();
    QString password1 = ui->repeatLE->text();

    if (password == password1)
    {
        emit sendUser(name, password);
        this->deleteLater();
        return;
    }

    QMessageBox msgBox;
    QString text = "Repeat your password properly!";
    msgBox.setText(text);
    msgBox.exec();
}

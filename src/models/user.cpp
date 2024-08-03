#include "user.h"
#include <QCryptographicHash>
#include <QString>

User::User(): _username(DEFAULT_USERNAME), _passwd(DEFAULT_PASSWD)
{ }

User::User(QString name, QString password):
    _username(name), _passwd(password)
{ }

User::~User() { }

bool User::checkPasswd(QString password)
{
    return _passwd == password;
}

QString User::username() {return _username; }
bool User::username(QString name, QString password)
{
    if (checkPasswd(password))
    {
        _username = name;
        return true;
    }
    return false;
}

QString User::passwd() {return _passwd; }

bool User::passwd(QString new_, QString old)
{
    if (checkPasswd(old))
    {
        _passwd = new_;
        return true;
    }
    return false;
}

const QString User::DEFAULT_USERNAME = "John Doe";
const QString User::DEFAULT_PASSWD = "password";


const QString User::defaultPasswd() {return DEFAULT_PASSWD;}

const QString User::defaultUsername() {return DEFAULT_USERNAME; }

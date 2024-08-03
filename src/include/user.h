#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    User(QString, QString);
    virtual ~User();
    bool checkPasswd(QString);

    QString username();
    bool username(QString newUsername, QString password);

    QString passwd();
    bool passwd(QString old, QString newPasswd);

    const QString defaultPasswd();
    const QString defaultUsername();

protected:
    QString _username;
    QString _passwd;

private:

    static const QString DEFAULT_USERNAME;
    static const QString DEFAULT_PASSWD;
};

#endif // USER_H

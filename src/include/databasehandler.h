#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H


#include "connection.h"
#include "singlequotemodel.h"
#include "user.h"
#include <QSqlDatabase>

class DatabaseHandler
{
public:
    DatabaseHandler();
    DatabaseHandler(std::shared_ptr<User>, std::shared_ptr<Connection>);
    virtual ~DatabaseHandler();

    enum CoreTables {owners, attrs, features};

    std::string user();

    bool auth();
    std::string lastErrorText();

    bool addUser(User*);
    QList<SingleQuoteModel> *getQuoteCards(const QString &owner);
    QList<QString>* getCoreTable(DatabaseHandler::CoreTables);
    QList<SingleQuoteModel>* getFavoriteQuotes();
    bool addFavorite(int q_id);

    bool addQuote(SingleQuoteModel* data);

    // QList<QString>* getAttrs();
    bool addOwner(std::pair<QString, QString>);
    bool deleteQuote(QString, QString, int);

private:
    std::unique_ptr<QSqlDatabase> db;
    std::shared_ptr<User> _user;
    std::shared_ptr<Connection> _conn;

    void setLogin();
};

#endif // DATABASEHANDLER_H

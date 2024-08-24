#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

class Connection
{
public:
    Connection();
    Connection(QString server, int port);
    virtual ~Connection();

    QString server();
    void server(QString);

    int port();
    void port(int);

    void setDefaultConn();
    int defaultPort();
    QString defaultServer();

private:
    QString _server;
    int _port;

    static const QString DEFAULT_SERVER;
    static const int DEFAULT_PORT;
};

#endif // CONNECTION_H

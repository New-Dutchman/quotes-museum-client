#include "connection.h"
#include "qdebug.h"


const QString Connection::DEFAULT_SERVER = "147.45.241.34";
const int Connection::DEFAULT_PORT = 8080;

Connection::Connection(): _server(DEFAULT_SERVER), _port(DEFAULT_PORT)
{

}
Connection::Connection(QString server, int port):
    _server(server), _port(port)
{

}

Connection::~Connection() { }

void Connection::setDefaultConn()
{
    _port = DEFAULT_PORT;
    _server = DEFAULT_SERVER;
}

QString Connection::server() {return _server; }
int Connection::port() {return _port; }

void Connection::server(QString newValue) {_server = newValue; }
void Connection::port(int newPort) {_port = newPort; }

QString Connection::defaultServer() {return DEFAULT_SERVER; }
int Connection::defaultPort() {return DEFAULT_PORT; }
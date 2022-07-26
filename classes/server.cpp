#include "server.h"

Server::Server(QObject* parent): QLocalServer{parent} {
    initSocket();
}

void Server::initSocket() {
    QString serverName = "http://localhost:8000/";

    socket = new QLocalSocket;
    socket->connectToServer(serverName, QIODeviceBase::WriteOnly);

    listen(serverName);
}

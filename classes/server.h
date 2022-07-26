#ifndef SERVER_H
#define SERVER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QIODeviceBase>

class Server : public QLocalServer {
    Q_OBJECT

    public:
        explicit Server(QObject* parent = nullptr);
        QLocalSocket* socket;

    private:
        void initSocket();
};

#endif // SERVER_H

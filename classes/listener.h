#ifndef LISTENER_H
#define LISTENER_H

#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <httplib.h>
#include <string>

using namespace httplib;

class Listener : public QThread, Server {
    Q_OBJECT

    public:
        Listener(QObject* parent = nullptr);
        ~Listener();

        signals:
            void rpcParamsSent(QJsonObject params);
            void command(QString cmd);

    protected:
        void run();

    private:
        void respond(const Request& req, Response& res);
};

#endif // LISTENER_H

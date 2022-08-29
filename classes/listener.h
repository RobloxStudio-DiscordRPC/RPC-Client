#ifndef LISTENER_H
#define LISTENER_H

#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <httplib.h>

using namespace httplib;

class Listener : public QThread, Server {
    Q_OBJECT

    public:
        Listener(QObject* parent = nullptr);
        ~Listener();

        static std::string fromHeaders(
            Headers headers,
            std::string key
        );

        signals:
            void rpcParamsSent(QJsonObject params);
            void command(QString cmd);

    protected:
        void run();

    private:
        void respond(const Request& req, Response& res);
};

#endif // LISTENER_H

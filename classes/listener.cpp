#include "listener.h"

Listener::Listener(QObject* parent): QThread(parent) {
    start();
    setPriority(Priority::IdlePriority);
}

void Listener::run() {
    Post("/rbxstudioDiscRPC", [this](const Request &req, Response &res){respond(req, res);});
    listen("localhost", 8000);
}

void Listener::respond(const Request &req, Response &res) {
    qDebug() << QString(req.body.data());
    const QByteArray data(req.body.data());
    const QJsonDocument json = QJsonDocument::fromJson(data);
    emit posted(json.object());
    res.set_content("success", "text/plain");
}

Listener::~Listener() {
    stop(); // stop listening

    if (isRunning()) {
        // terminate thread if running
        quit();
        wait();
    }

    // thank god these names are different :troll:
}

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
    const std::string contentType = req.headers["Content-Type"];
    const QString body(req.body.data());
    qDebug() << body;
    if (contentType == "application/jsom") {
        emit rpcParamsSent(QJsonDocument::fromJson(QByteArray(body)).object());
    } else if ((contentType == "text/plain") && body.startsWith('!')) {
        emit command(body.remove(0,1));
    }

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

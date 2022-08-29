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
    const std::string contentType = fromHeaders(req.headers, "Content-Type");

    const QString body(req.body.data());
    qDebug() << body;

    if (contentType == "application/json") {
        emit rpcParamsSent(QJsonDocument::fromJson(body.toUtf8()).object());

    } else if ((contentType == "text/plain") && body.startsWith('!')) {

        emit command(body.mid(1));
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

std::string Listener::fromHeaders(Headers headers, std::string key) {
    for (auto& iter : headers) if (iter.first == key) return iter.second;
    return NULL;
}

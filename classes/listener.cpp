#include "listener.h"

Listener::Listener(QObject* parent): QThread(parent) {
    start();
}

void Listener::run() {
    Post("/", [this](const Request &req, Response &res){respond(req, res);});
    listen("localhost", 8000);
}

void Listener::respond(const Request &req, Response &res) {
    req.body; // do not mark this as unused
    emit posted();
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

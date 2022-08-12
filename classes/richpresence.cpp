#include "richpresence.h"
#include <thread>

RichPresence::RichPresence(QObject *parent): QThread{parent} {
    initDiscord();

    if (coreError==Result::Ok) {
        initActivity();
    }
}

RichPresence::~RichPresence() {
    if (isRunning()) {
        stop();
        wait();
    }

    delete discordCore;
}

void RichPresence::errorMsg(const QString msg, const Result errCode) {
    if (errCode == Result::Ok) return;

    const QString txt = (
        msg +
        QString(" Error code: ") +
        QString::number((int) errCode)
    );

    // will error because it tries to make gui in thread that isn't main thread
    /*
    QMessageBox::critical(
        nullptr,
        "Discord rich presence error occured!",
        txt,
        "Quit application"
    );

    */
    qDebug() << txt;

    //QCoreApplication::instance()->quit();
}

void RichPresence::initDiscord() {
    Core* core{};
    discordCore = core;
    Result resp = Core::Create(
        appId,
        DiscordCreateFlags_NoRequireDiscord,
        &discordCore
    );

    if (resp != Result::Ok) {
        RichPresence::errorMsg("Couldn't initialize the Discord service!", resp);
    }

    if (!discordCore) {
        resp = Result::InternalError;
    }

    coreError = resp;

    if (resp == Result::Ok) {
        manager = &discordCore->ActivityManager();
    } else {
        activity = NULL;

        delete discordCore;
        discordCore = NULL;
        manager = NULL;
    }
}

void RichPresence::initActivity() {
    activity = new Activity{};
    activity->SetDetails("Idling");
    //activity->SetState("");
    activity->GetAssets().SetLargeImage("rbxstudio");
    activity->GetAssets().SetLargeText("Roblox Studio");
    activity->SetType(ActivityType::Playing);
    activity->GetTimestamps().SetStart(QDateTime::currentMSecsSinceEpoch());

    updateActivity();
}

void RichPresence::updateActivity() {
    manager->UpdateActivity(*activity, [this](const Result result) {
        rpcError = result;
        //RichPresence::errorMsg("Couldn't update rich presence!", result);
    });
}

void RichPresence::start() {
    isStopped = false;
    QThread::start();
    setPriority(Priority::IdlePriority);
}

void RichPresence::run() {
    do {
        pollError = discordCore->RunCallbacks();
        QThread::msleep(16);
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!isStopped);
}

void RichPresence::stop() {
    isStopped = true;
}

#include "richpresence.h"
#include <thread>

RichPresence::RichPresence(QObject *parent): QThread{parent} {
    initDiscord();
    initActivity();
}

RichPresence::~RichPresence() {
    if (isRunning()) {
        stop();
        wait();
    }

    // delete manager;
    /**
        deleting the (pointer to the) manager causes an error to occur
        investigating later
    */

    discordCore.reset(); // deinit (destroy) core
}

void RichPresence::errorMsg(const QString msg, const Result errCode, const bool quit) {
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

    if (quit) {
        QCoreApplication::instance()->quit();
    }
}

void RichPresence::initDiscord() {
    Core* core{};
    Result resp = Core::Create(
        appId,
        DiscordCreateFlags_NoRequireDiscord,
        &core
    );

    if (resp != Result::Ok) {
        RichPresence::errorMsg("Couldn't initialize the Discord service!", resp);
    }

    discordCore.reset(core);

    manager = &discordCore->ActivityManager();
}

void RichPresence::initActivity() {
    activity = new Activity{};
    activity->SetName("name");
    activity->SetDetails("Working on: X");
    activity->SetState("Editing script: Y");
    //activity->GetAssets().SetSmallImage("rbxstudio");
    activity->GetAssets().SetSmallText("Roblox Studio");
    //activity->GetAssets().SetLargeImage("script");
    activity->GetAssets().SetLargeText("Y");
    activity->SetType(ActivityType::Playing);
    //activity->GetSecrets().SetJoin("Go to game");
    //activity->GetSecrets().SetSpectate("GitHub repository");

    manager->UpdateActivity(*activity, [this](const Result result) {
        qDebug() << QString(activity->GetName());
        RichPresence::errorMsg("Couldn't update rich presence!", result, false);
    });
}

void RichPresence::start() {
    isStopped = false;
    QThread::start();
}

void RichPresence::run() {
    do {
        msleep(16);
        discordCore->RunCallbacks();
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!isStopped);
}

void RichPresence::stop() {
    isStopped = true;
}
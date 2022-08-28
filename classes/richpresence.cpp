#include "richpresence.h"

RichPresence::RichPresence(QObject *parent): QThread{parent} {
    coreError = Result::Ok;
    pollError = Result::Ok;
    rpcError  = Result::Ok;

    initActivity();
}

RichPresence::~RichPresence() {
    if (isRunning()) {
        stop();
        // note: stop() deinits discord core
    }

    delete activity;
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

void RichPresence::deinitDiscord() {
    delete discordCore;
    discordCore = NULL;
    manager = NULL;
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
        deinitDiscord();
    }
}

void RichPresence::initActivity() {
    activity = new Activity{};
    activity->SetDetails("Idling");
    //activity->SetState("");
    activity->GetAssets().SetLargeImage("rbxstudio");
    activity->GetAssets().SetLargeText("Roblox Studio");
    activity->SetType(ActivityType::Playing);
    resetElapsedTimer();
}

void RichPresence::resetElapsedTimer() {
    activity->GetTimestamps().SetStart(QDateTime::currentMSecsSinceEpoch());
}

void RichPresence::updateActivity() {
    manager->UpdateActivity(*activity, [this](const Result result) {
        rpcError = result;
        //RichPresence::errorMsg("Couldn't update rich presence!", result);
    });
}

void RichPresence::start() {
    if (discordCore == NULL) {
        initDiscord();
        if (coreError != Result::Ok) return;
        updateActivity();
    }

    isStopped = false;
    pollError = Result::Ok;
    QThread::start();

    setPriority(Priority::IdlePriority);
}

void RichPresence::run() {
    updateActivity();

    while (!isStopped) {
        if (pollError != Result::Ok) {
            isStopped = true;
            break;
        }

        pollError = discordCore->RunCallbacks();

        qDebug() << "callbacks" << (int) pollError;

        QThread::msleep(200);
    }

    qDebug() << "no callbacks";
}

void RichPresence::stop() {
    isStopped = true;

    // if is running wait until it finishes
    if (isRunning()) {
        QEventLoop waiter(this);
        connect(
            this, &RichPresence::finished,
            &waiter, &QEventLoop::quit
        );
        waiter.exec();
    }

    if (discordCore != NULL) deinitDiscord();
}

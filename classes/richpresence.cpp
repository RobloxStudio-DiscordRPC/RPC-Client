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

void RichPresence::deinitDiscord() {
    delete discordCore;
    discordCore = NULL;
}

void RichPresence::initDiscord() {
    Core* core{}; discordCore = core; // bruh
    Result resp = Core::Create(
        appId,
        DiscordCreateFlags_NoRequireDiscord,
        &discordCore // wtf! bro wants a pointer to a pointer!
    );

    if (!discordCore) {
        resp = Result::InternalError;
    }

    coreError = resp;

    if (resp != Result::Ok) {
        deinitDiscord();
    }
}

void RichPresence::initActivity() {
    activity = new Activity{};
    activity->SetDetails("Idling");
    activity->GetAssets().SetLargeImage("rbxstudio");
    activity->GetAssets().SetLargeText("Roblox Studio");
    activity->SetType(ActivityType::Playing);
    resetElapsedTimer();
}

void RichPresence::resetElapsedTimer() {
    activity->GetTimestamps().SetStart(QDateTime::currentMSecsSinceEpoch());
}

void RichPresence::updateActivity() {
    discordCore->ActivityManager().UpdateActivity(*activity, [this](const Result result) {
        rpcError = result;
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

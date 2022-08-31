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
    setActivityIdle();
}

void RichPresence::setActivityIdle() {
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
void RichPresence::processCommand(QString cmd) {
    // not many commands for now, remember to add a switch case
    /*
    QStringList commands;
    commands << "IDLE";
    */
    if (cmd == "IDLE") setActivityIdle();
}

void RichPresence::processParams(QJsonObject params) {
    const QJsonObject formats = params["FORMATS"].toObject();

    const QString projName = params["PROJECT"].toString();
    activity->SetDetails(formats["DETAILS"].toString().replace(REPLACER, projName).toUtf8().constData());
    activity->GetAssets().SetLargeText(formats["STATE"]["LARGE"].toString().toUtf8().constData());

    const QJsonObject editing = params["EDITING"].toObject();
    if (editing.length() > 0) {
        const QString name = editing["NAME"].toString();
        const QString type = editing["TYPE"].toString();
        QString statusFormat = formats["STATE"][type].toString();

        // bruh "c++ doesn't support string switch cases"
        QStringList typeSwitchCase;
        typeSwitchCase << "SCRIPT" << "GUI" << "BUILD";
        QString iconName;
        switch (typeSwitchCase.indexOf(type)) {
            case 0: // script
                iconName = editing["CLASS"].toString().toLower();
                break;

            case 1: // gui
                iconName = "gui";
                break;

            case 2: // build
                iconName = "model";
                break;

            default:
                statusFormat = formats["STATE"]["DEFAULT"].toString();
                iconName = type.toLower();
                break;
        }

        //TODO: change name of model asset

        activity->SetState(statusFormat.replace(REPLACER, name).toUtf8().constData());
        activity->GetAssets().SetSmallImage(iconName.toUtf8().constData());
        activity->GetAssets().SetSmallText(formats["ASSETS"]["SMALL"].toString().replace(REPLACER, name).toUtf8().constData());
    } else {
        activity->SetState("");
        activity->GetAssets().SetSmallImage("workspace");
        activity->GetAssets().SetSmallText(formats["ASSETS"]["SMALL"].toString().replace(REPLACER, projName).toUtf8().constData());
    }

    // will fail building without this->???
    this->resetElapsedTimer();
    this->updateActivity();
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

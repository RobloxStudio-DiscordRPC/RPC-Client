#ifndef RICHPRESENCE_H
#define RICHPRESENCE_H

#include <QThread>
#include <discord.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QTimer>
#include <QDateTime>

using namespace discord;

class RichPresence : public QThread {
    Q_OBJECT

    public:
        RichPresence(QObject *parent = nullptr);
        ~RichPresence();

        const ClientId appId = 1002325872545644574;

        Activity* activity;
        void updateActivity();

        discord::Result coreError;
        discord::Result  rpcError;
        volatile discord::Result pollError;

        void start();
        void stop ();

    protected:
        void run();

    private:
        void errorMsg(
            const QString msg,
            const discord::Result errCode
        );

        Core* discordCore;
        void initDiscord();
        void deinitDiscord();

        ActivityManager* manager;
        void initActivity();

        bool isStopped = false;
};

#endif // RICHPRESENCE_H

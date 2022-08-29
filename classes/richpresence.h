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
        void resetElapsedTimer();

        discord::Result coreError;
        discord::Result  rpcError;
        volatile discord::Result pollError;

        void start();
        void stop ();

        void initDiscord();
        void deinitDiscord();

    protected:
        void run();

    private:
        Core* discordCore;
        void initActivity();

        bool isStopped = false;
};

#endif // RICHPRESENCE_H

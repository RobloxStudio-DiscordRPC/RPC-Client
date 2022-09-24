#ifndef RICHPRESENCE_H
#define RICHPRESENCE_H

#include <QThread>
#include <discord.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QJsonObject>
#include <QTimer>
#include <QDateTime>

#define REPLACER "{}"

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

        public slots:
            void processCommand(QString cmd);
            void processParams(QJsonObject params);

            void setActivityIdle();

    protected:
        void run();

    private:
        Core* discordCore;
        void initActivity();

        bool isStopped = false;
};

#endif // RICHPRESENCE_H

#ifndef RICHPRESENCE_H
#define RICHPRESENCE_H

#include <QThread>
#include <discord.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QTimer>

using namespace discord;

class RichPresence : public QThread {
    Q_OBJECT

    public:
        RichPresence(QObject *parent = nullptr);
        ~RichPresence();

        const ClientId appId = 1002325872545644574;

        Activity* activity;
        void updateActivity();

        void start();
        void stop ();

    protected:
        void run();

    private:
        static void errorMsg(
            const QString msg,
            const discord::Result errCode,
            const bool quit = true
        );

        std::unique_ptr<Core> discordCore;
        void initDiscord();

        ActivityManager* manager;
        void initActivity();

        bool isStopped = false;
};

#endif // RICHPRESENCE_H

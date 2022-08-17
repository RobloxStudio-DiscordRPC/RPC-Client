#ifndef PROCESSTRACKER_H
#define PROCESSTRACKER_H

#include <QThread>
#include <QtDebug>

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
    #include <tchar.h>

    #define PID_NOT_FOUND 0
#endif

class ProcessTracker : public QThread {
    Q_OBJECT

    public:
        explicit ProcessTracker(QString pname, QObject *parent = nullptr);

        QString pName;
        int pPid;
        void refreshPid();

        static int getPidByName(const QString pname);

        void start();

        signals:
            void stateChanged(bool isRunning);

    protected:
        void run();

    private:
        HANDLE pHandle;
};

#endif // PROCESSTRACKER_H

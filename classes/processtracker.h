#ifndef PROCESSTRACKER_H
#define PROCESSTRACKER_H

#include <QThread>
#include <QtDebug>

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
    #include <tchar.h>
    #include <functional>

    #define PID_NOT_FOUND -1
#endif

typedef tagPROCESSENTRY32W Process;
typedef std::function<bool(Process)> ProcessLoop;

class ProcessTracker : public QThread {
    Q_OBJECT

    public:
        explicit ProcessTracker(QString pname, QObject *parent = nullptr);

        QString pName;
        int pPid;
        void refreshPid();
        bool isProcessRunning();

        static int getPidByName(const QString pname);

        void start();

        signals:
            void stateChanged(bool isRunning);

    protected:
        void run();

    private:
        HANDLE pHandle;
        bool waiting;

        static void loopThroughProcesses(ProcessLoop callback);
};

#endif // PROCESSTRACKER_H

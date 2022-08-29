#ifndef PROCESSTRACKER_H
#define PROCESSTRACKER_H

#include <QThread>
#include <QtDebug>
#include <QEventLoop>

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
    #include <tchar.h>
    #include <functional>

    #define PID_NOT_FOUND -1
    #define POLLING_INTERVAL 2000
#endif

struct Process {
    QString name;
    int pid = PID_NOT_FOUND;
    bool running = false;
    HANDLE handle = NULL;
};

typedef tagPROCESSENTRY32W ProcessInfo;
typedef std::function<bool(ProcessInfo)> ProcessLoop;

class ProcessTracker : public QThread {
    Q_OBJECT

    public:
        ProcessTracker(QString pname, QObject *parent = nullptr);
        ~ProcessTracker();

        Process proc;
        void refreshPid();
        bool isProcessRunning();

        static int getPidByName(const QString pname);

        void start();
        void stop();

        signals:
            void stateChanged(bool isRunning);

    protected:
        void run();

    private:
        bool looping = false;

        static void loopThroughProcesses(ProcessLoop callback);
};

#endif // PROCESSTRACKER_H

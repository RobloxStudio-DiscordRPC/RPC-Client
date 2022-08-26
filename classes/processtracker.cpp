#include "processtracker.h"

ProcessTracker::ProcessTracker(QString pname, QObject *parent): QThread{parent} {
    proc.name = pname;
    refreshPid();
}

ProcessTracker::~ProcessTracker() {
    if (isRunning()) stop();
}

void ProcessTracker::refreshPid() {
    proc.pid = getPidByName(proc.name);
}

void ProcessTracker::loopThroughProcesses(ProcessLoop callback) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        bool loopBreak = false;

        do {
            loopBreak = callback(entry);
        } while (Process32Next(snapshot, &entry) && (!loopBreak));
    }

    CloseHandle(snapshot);
}

int ProcessTracker::getPidByName(const QString name) {
    int pid = PID_NOT_FOUND;

    loopThroughProcesses([&pid, name](ProcessInfo pi) -> bool {
        if (!_tcsicmp(pi.szExeFile, name.toStdWString().c_str())) {
            pid = pi.th32ProcessID;
            return true;
        }

        return false;
    });

    return pid;
}

bool ProcessTracker::isProcessRunning() {
    // if tracking, return result of tracker
    if (isRunning()) return proc.running;

    bool running = false;

    loopThroughProcesses([this, &running](ProcessInfo pi) {
        if (!_tcsicmp(pi.szExeFile, proc.name.toStdWString().c_str())) running = true;
        return running;
    });

    return running;
}

void ProcessTracker::start() {
    QThread::start();
    setPriority(Priority::IdlePriority);
}

void ProcessTracker::run() {
    refreshPid();
    looping = true;

    while (looping) {
        proc.handle = OpenProcess(SYNCHRONIZE, TRUE, proc.pid);

        if (proc.handle != NULL) {

            proc.running = true;
            emit stateChanged(proc.running);
            //WaitForSingleObject(proc.handle, INFINITE);
            while (
                looping &&
                (WaitForSingleObject(proc.handle, EMPTY_TIMEOUT) == WAIT_OBJECT_0)
            ) QThread::msleep(2000);

            CloseHandle(proc.handle);
            proc.handle = NULL;

        } else {
            proc.pid = PID_NOT_FOUND;

            proc.running = false;
            emit stateChanged(proc.running);
            while (looping) {
                refreshPid();
                if (proc.pid != PID_NOT_FOUND) break;
                QThread::msleep(2000);
            }
        }
    }
}

void ProcessTracker::stop() {
    looping = false;

    // if is running wait until it finishes
    if (isRunning()) {
        QEventLoop waiter(this);
        connect(
            this, &ProcessTracker::finished,
            &waiter, &QEventLoop::quit
        );
        waiter.exec();
    }
}

#include "processtracker.h"

ProcessTracker::ProcessTracker(QString pname, QObject *parent): QThread{parent} {
    pName = pname;
    refreshPid();
}

void ProcessTracker::refreshPid() {
    pPid = getPidByName(pName);
}

void ProcessTracker::loopThroughProcesses(ProcessLoop callback) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        bool loopBreak = false;

        do {
            loopBreak = callback(entry);
        } while (Process32Next(snapshot, &entry) || (!loopBreak));
    }

    CloseHandle(snapshot);
}

int ProcessTracker::getPidByName(const QString name) {
    int pid = PID_NOT_FOUND;

    /*
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        do {
            if (!_tcsicmp(entry.szExeFile, name.toStdWString().c_str())) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    */

    loopThroughProcesses([&pid, name](Process process) -> bool {
        if (!_tcsicmp(process.szExeFile, name.toStdWString().c_str())) {
            pid = process.th32ProcessID;
            return true;
        }

        return false;
    });

    return pid;
}

bool ProcessTracker::isProcessRunning() {
    if (isRunning() && waiting) return true;

    bool running = false;

    return running;
}

void ProcessTracker::start() {
    QThread::start();
    setPriority(Priority::IdlePriority);
}

void ProcessTracker::run() {
    pHandle = OpenProcess(SYNCHRONIZE, TRUE, pPid);
    if (pHandle != NULL) {

        waiting = true;
        WaitForSingleObject(pHandle, 0);
        waiting = false;

        CloseHandle(pHandle);

    } else {
        qDebug() << "could not init process handle, error code:" << QString::number(GetLastError()).toStdString().c_str();
    }
}

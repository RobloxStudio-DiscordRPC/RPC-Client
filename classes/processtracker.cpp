#include "processtracker.h"

ProcessTracker::ProcessTracker(QString pname, QObject *parent): QThread{parent} {
    pName = pname;
    refreshPid();
}

void ProcessTracker::refreshPid() {
    pPid = getPidByName(pName);
}

int ProcessTracker::getPidByName(const QString name) {
    int pid = PID_NOT_FOUND;

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

    /*
    HANDLE hSnapshot;
    PROCESSENTRY32 pe;
    int pid = PID_NOT_FOUND;
    BOOL hResult;

    // snapshot of all processes in the system
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

    // initializing size: needed for using Process32First
    pe.dwSize = sizeof(PROCESSENTRY32);

    // info about first process encountered in a system snapshot
    hResult = Process32First(hSnapshot, &pe);

    // retrieve information about the processes
    // and exit if unsuccessful
    while (hResult) {
        // if we find the process: return process ID
        if (!_tcsicmp(cname, pe.szExeFile) == 0) {
            pid = pe.th32ProcessID;
            break;
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    // closes an open handle (CreateToolhelp32Snapshot)
    CloseHandle(hSnapshot);
    */

    return pid;
}


void ProcessTracker::start() {
    QThread::start();
    setPriority(Priority::IdlePriority);
}

void ProcessTracker::run() {
    pHandle = OpenProcess(SYNCHRONIZE, TRUE, pPid);
    if (pHandle != NULL) {
        WaitForSingleObject(pHandle, 0);
        CloseHandle(pHandle);
    } else {
        qDebug() << "could not init process handle, error code:" << QString::number(GetLastError()).toStdString().c_str();
    }
}

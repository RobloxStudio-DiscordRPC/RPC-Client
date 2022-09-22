#include "processtracker.h"

ProcessTracker::ProcessTracker(QString pname, QObject *parent): QThread{parent} {
    proc.name = pname;
    proc.pid=getPidByName(proc.name);

    waitstop = CreateEvent(NULL, TRUE, FALSE, NULL);
}

ProcessTracker::~ProcessTracker() {
    if (isRunning()) stop();
    CloseHandle(waitstop);
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
    bool looping = true;
    HANDLE procHandle = NULL;
    proc.pid=getPidByName(proc.name);

    while (looping) {
        procHandle = OpenProcess(SYNCHRONIZE, TRUE, proc.pid);

        proc.running = (procHandle != NULL);
        emit stateChanged(proc.running);

        if (proc.running) {

            HANDLE events[2];
            events[0] = procHandle;
            events[1] = waitstop;
            switch (WaitForMultipleObjects(2, events, FALSE, INFINITE))
            {
                case WAIT_OBJECT_0:   //Proc.handle
                    break;
                case WAIT_OBJECT_0+1: //WaitStop Event
                case WAIT_FAILED:
                    looping=false;
                    break;
                default:
                    break;
            }
            CloseHandle(procHandle);
            procHandle = NULL;

        } else {
            // active process search by name
            while (looping){
                proc.pid=getPidByName(proc.name);
                if (proc.pid==PID_NOT_FOUND){
                    switch (WaitForSingleObject(waitstop,POLLING_INTERVAL)){
                        case WAIT_TIMEOUT: break;
                        case WAIT_OBJECT_0:
                            looping=false;
                            break;
                        default:
                            //TODO: Report the error!!!
                            //QThread::msleep(POLLING_INTERVAL);
                            looping=false;
                            break;
                    }

                }else break;
            }
        }
    }
    //this->quit();
}

void ProcessTracker::stop() {
    if (!isRunning()) return;

    SetEvent(waitstop);

    wait();

}

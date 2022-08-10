#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    richPresence=NULL;
    server=NULL;

    trayicon = new SystemTrayIcon;
    trayicon->show();
    trayicon->showMessage("hello","start listening!",trayicon->icon());

    initServer();

    initRichPresence();

    /*
    connect(
        ui->exitBtn, &QPushButton::pressed,
        QApplication::instance(), &QCoreApplication::exit
    );
    */

    connect(
        ui->dscReconnectBtn, &QPushButton::pressed,
        this, &MainWindow::initRichPresence
    );

    refreshRbxStudio();
    connect(
        ui->rsReconnectBtn, &QPushButton::pressed,
        this, &MainWindow::refreshRbxStudio
    );
}

MainWindow::~MainWindow() {
    safedelete(server);
    safedelete(trayicon);
    safedelete(ui);
}

void MainWindow::initServer() {
    server = new Listener(this);
    connect(
        server, &Listener::posted,
        this, &MainWindow::loadParams
    );
}

void MainWindow::loadParams(QJsonObject params) {

    const bool editingExists = params.contains("EDITING");
    if (editingExists) {
        const QJsonValue  editingVal = params.value("EDITING");
        const QJsonObject editing = editingVal.toObject();
        const QString name = editing.value("NAME").toString();
        const QString type = editing.value("TYPE").toString();

        const QString out = QString("name: %1, type: %2").arg(name,type);

        Activity* activity = richPresence->activity;
        activity->SetState(("Editing script: " + name).toLocal8Bit().data());
        activity->GetAssets().SetSmallImage(type.toLower().toStdString().data());
        activity->GetAssets().SetSmallText(name.toStdString().data());
        richPresence->updateActivity();

        trayicon->showMessage("update rich presence", out);
    }
}

void MainWindow::initRichPresence() {
    if (richPresence!=NULL){
        delete richPresence;
        richPresence=NULL;
    }
    richPresence = new RichPresence(this);

    const discord::Result error = richPresence->coreError;
    if (error == discord::Result::Ok) {
        ui->dscFound->setText("Discord found!");
        richPresence->start();
    } else {
        switch (error) {

            case discord::Result::InternalError:
                ui->dscFound->setText("Discord not found!");
                break;

            default:
                const int code = (int) error;
                const QString msg = "An error has occured! Error code: "+QString::number(code);

                /*
                if error code is invalid (is invalid enum)
                then don't show an error message and instead print warning

                43 because that's where the discord::Result enum ends
                */
                if ((code < 0) || (code > 43)) {
                    qWarning() << msg;
                    break;
                };

                QMessageBox::critical(
                    nullptr,
                    "Discord RPC",
                    msg,
                    "Ok"
                );
                break;
        }
    }
}

bool MainWindow::setRbxStudioFound(const bool val) {
    rbxStudioFound = val;
    return rbxStudioFound;
}

bool MainWindow::refreshRbxStudio() {
    const QString process = "RobloxStudioBeta.exe";

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    rbxStudioFound = false;

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        do {
            if (!_tcsicmp(entry.szExeFile, process.toStdWString().c_str())) {
                rbxStudioFound = true;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    ui->rbxstudioFound->setText(rbxStudioFound ? "Roblox Studio found!" : "Roblox Studio not found!");
    return rbxStudioFound;

    /*
    QProcess rbxStudio(this);
    rbxStudio.setProgram("RobloxStudioBeta.exe");

    rbxStudioFound = rbxStudio.state() != QProcess::NotRunning;
    ui->rbxstudioFound->setText(rbxStudioFound ? "Roblox Studio found!" : "Roblox Studio not found!");

    //delete rbxStudio;
    */
}

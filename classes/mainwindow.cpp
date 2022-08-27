#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    richPresence=NULL;
    server=NULL;

    trayicon = new SystemTrayIcon;
    trayicon->show();
    connect(
        trayicon->openWindow, &QAction::triggered,
        this, [this](){
            show();
            setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
            ui->tabs->setCurrentIndex(0);
        }
    );
    connect(
        trayicon->about, &QAction::triggered,
        this, [this](){
            show();
            setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
            ui->tabs->setCurrentIndex(2);
        }
    );

    initSettings();

    initServer();

    initRichPresence();

    connect(
        ui->exitBtn, &QPushButton::pressed,
        trayicon, &SystemTrayIcon::quitApp
    );

    connect(
        ui->dscStatus, &FindStatusDisplay::research,
        this, &MainWindow::initRichPresence
    );

    initRbxStudioTracker();
    refreshRbxStudio();
    connect(
        ui->rbxstudioStatus, &FindStatusDisplay::research,
        this, &MainWindow::refreshRbxStudio
    );
    connect(
        rbxStudioTracker, &ProcessTracker::stateChanged,
        this, &MainWindow::updateRobloxStudioStatus
    );

    ui->launchOnLoginCheck->setChecked(QFile::exists(getLoginLaunchLnkPath()));
    connect(
        ui->launchOnLoginCheck, &QCheckBox::toggled,
        this, &MainWindow::setStartOnLogin
    );

    connect(
        QApplication::instance(), &QCoreApplication::aboutToQuit,
        settings, &Settings::write
    );
}

MainWindow::~MainWindow() {
    safedelete(server);
    safedelete(settings);
    trayicon->hide();
    safedelete(trayicon);
    safedelete(richPresence);
    safedelete(rbxStudioTracker);
    safedelete(ui);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (settings->get("EXIT_ON_CLOSE").toBool()) {
        event->accept();
        return;
    }

    event->ignore();

    hide();
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

    richPresence->initDiscord();
    //if (rbxStudioTracker->isRunning()) richPresence->start();
    refreshDiscord();

    const discord::Result error = richPresence->coreError;
    switch (error) {
        case discord::Result::Ok:
            qDebug("ok");
            connect(
                richPresence, &RichPresence::finished,
                this, &MainWindow::refreshDiscord
            );
            break;

        case discord::Result::InternalError:
            qDebug("case");
            break;

        default:
            qDebug("def");
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
                this,
                "Discord RPC",
                msg,
                "Ok"
            );
            break;
    }
}

void MainWindow::refreshDiscord() {
    ui->dscStatus->setFound(
        richPresence->coreError == discord::Result::Ok &&
        richPresence->rpcError  == discord::Result::Ok &&
        richPresence->pollError == discord::Result::Ok
    );
}

QFileInfo MainWindow::getExecutableInfo() {
    return QFileInfo(QCoreApplication::applicationFilePath());
}

void MainWindow::initSettings() {
    settings = new Settings(new QFile(
        getExecutableInfo().absolutePath() +
        QDir::separator() +
        "rsdrpc_config.json"
    ));

    ui->closeExitCheck->setChecked(settings->get("EXIT_ON_CLOSE").toBool());
    connect(
        ui->closeExitCheck, &QCheckBox::toggled,
        this, [this](const bool exitOnClose){
            settings->set("EXIT_ON_CLOSE", exitOnClose);
        }
    );
}

void MainWindow::initRbxStudioTracker() {
    rbxStudioTracker = new ProcessTracker("RobloxStudioBeta.exe", this);
    rbxStudioTracker->start();
}

void MainWindow::updateRobloxStudioStatus(const bool status) {
    ui->rbxstudioStatus->setFound(status);
    if (status) {
        if (!richPresence->isRunning()) richPresence->start();
    } else if (status) {
        richPresence->stop();
    }
}

bool MainWindow::refreshRbxStudio() {
    const bool running = rbxStudioTracker->isProcessRunning();
    updateRobloxStudioStatus(running);
    return running;
}

QString MainWindow::getLoginLaunchLnkPath() {
    QFileInfo fileInfo = getExecutableInfo();
    QStringList appdata = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
    return (
        appdata.first() +
        QDir::separator() +
        "Startup" +
        QDir::separator() +
        fileInfo.completeBaseName() +
        ".lnk"
    );
}

void MainWindow::setStartOnLogin(bool s) {
    ui->launchOnLoginCheck->setChecked(s);
    emit startOnLoginChanged();

    QString lnk = getLoginLaunchLnkPath();

    if (s) {
        QFile::link(
            QCoreApplication::applicationFilePath(),
            lnk
        );
    } else {
        QFile::remove(lnk);
    }
}

bool MainWindow::startOnLogin() {return ui->launchOnLoginCheck->isChecked();}

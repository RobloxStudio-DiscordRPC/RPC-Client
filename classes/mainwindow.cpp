#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    trayicon = new SystemTrayIcon;
    trayicon->show();
    trayicon->showMessage("hello","start listening!",trayicon->icon());

    initServer();

    initRichPresence();
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

        const QString out = QString("name: %1, type: %2").arg(
            editing.value("NAME").toString(),
            editing.value("TYPE").toString()
        );

        trayicon->showMessage("update rich presence", out);
    }
}

void MainWindow::initRichPresence() {
    richPresence = new RichPresence(this);
    richPresence->start();
}

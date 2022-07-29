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
        this, [this](){trayicon->showMessage("whoop!","request!",trayicon->icon());}
    );
}

void MainWindow::initRichPresence() {
    richPresence = new RichPresence(this);
    richPresence->start();
}

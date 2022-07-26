#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    trayicon = new SystemTrayIcon;
    trayicon->show();
    //trayicon->showMessage("message","body",trayicon->icon());

    initServer();
}

MainWindow::~MainWindow() {
    safedelete(server);
    safedelete(trayicon);
    safedelete(ui);
}

void MainWindow::initServer() {
    server = new Server(this);
    qDebug() << "server:" << server->serverName() << '(' << server->errorString() << ')';
}

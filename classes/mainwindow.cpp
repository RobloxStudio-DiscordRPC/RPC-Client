#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    trayicon = new SystemTrayIcon;
    trayicon->show();
    trayicon->showMessage("hello","start listening!",trayicon->icon());

    initServer();

    connect(
        server, &Listener::posted,
        this, [this](){trayicon->showMessage("whoop!","request!",trayicon->icon());}
    );
}

MainWindow::~MainWindow() {
    safedelete(server);
    safedelete(trayicon);
    safedelete(ui);
}

void MainWindow::initServer() {
    server = new Listener(this);

    /*
    qDebug() << "server:" << server->serverName() << '(' << server->errorString() << ')';

    connect(
        server, &Server::newConnection,
        this, [this](){trayicon->showMessage("whoop!","new connection!",trayicon->icon());}
    );
    */
}

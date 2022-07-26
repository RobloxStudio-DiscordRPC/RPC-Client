#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    trayicon = new SystemTrayIcon;
    trayicon->show();
    trayicon->showMessage("message","body",trayicon->icon());
}

MainWindow::~MainWindow() {
    safedelete(trayicon);
    safedelete(ui);
}


#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(): QSystemTrayIcon() {
    setIcon(QIcon(":/app.png"));
    initMenus();
    connect(
        this, &SystemTrayIcon::activated,
        openWindow, [this](const SystemTrayIcon::ActivationReason reason){
            if (reason == SystemTrayIcon::Trigger) {
                openWindow->trigger();
            }
        }
    );
}

void SystemTrayIcon::initMenus() {
    menu = new QMenu();

    openWindow = new QAction("Open window", menu);
    menu->addAction(openWindow);
    menu->setDefaultAction(openWindow);

    about = new QAction("About", menu);
    menu->addAction(about);

    menu->addSeparator();

    quit = new QAction("Quit", menu);
    connect(
        quit, &QAction::triggered,
        this, &SystemTrayIcon::quitApp
    );
    menu->addAction(quit);

    setContextMenu(menu);
}

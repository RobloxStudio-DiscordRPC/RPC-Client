#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStyle>
#include <QMenu>

class SystemTrayIcon : public QSystemTrayIcon {
    Q_OBJECT

    public:
        SystemTrayIcon();

        QMenu* menu;
        QAction* openWindow;
        QAction* about;
        QAction* quit;

        inline void quitApp() {
            QApplication::quit();
        };

    private:
        void initMenus();
};

#endif // SYSTEMTRAYICON_H

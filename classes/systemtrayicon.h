#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStyle>
#include <QMenu>

#define safedelete(x) { \
    if ((x)!=NULL) { \
        delete (x); \
        (x)=NULL; \
    } \
}

class SystemTrayIcon : public QSystemTrayIcon {
    Q_OBJECT

    public:
        SystemTrayIcon();
        ~SystemTrayIcon();

        QMenu* menu;
        QAction* openWindow;
        QAction* about;
        QAction* quit;

        inline void quitApp() {
            QApplication::quit();
        }

    private:
        void initMenus();
};

#endif // SYSTEMTRAYICON_H

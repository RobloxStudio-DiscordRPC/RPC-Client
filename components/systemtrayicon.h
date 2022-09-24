#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QStyle>
#include <QMenu>

#ifndef safedelete
#define safedelete(x) { \
    if ((x)!=NULL) { \
        delete (x); \
        (x)=NULL; \
    } \
}
#endif

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

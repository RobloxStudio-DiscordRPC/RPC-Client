#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QStyle>
#include <QApplication>

class SystemTrayIcon : public QSystemTrayIcon {
    Q_OBJECT

    public:
        SystemTrayIcon();

    private:
        QApplication* app;
};

#endif // SYSTEMTRAYICON_H

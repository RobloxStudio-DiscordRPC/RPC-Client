#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <classes/listener.h>
#include <classes/systemtrayicon.h>
#include <classes/richpresence.h>
#include <classes/findstatusdisplay.h>
#include <classes/settings.h>
#include <classes/processtracker.h>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QCloseEvent>

#define safedelete(x) { \
    if ((x)!=NULL) { \
        delete (x); \
        (x)=NULL; \
    } \
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(bool startOnLogin READ startOnLogin WRITE setStartOnLogin NOTIFY startOnLoginChanged)

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        Settings* settings;

        bool rbxStudioFound;

        bool startOnLogin();

        signals:
            void startOnLoginChanged();

        public slots:
            void refreshDiscord();
            bool refreshRbxStudio();
            void setStartOnLogin(bool s);

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        Ui::MainWindow *ui;

        SystemTrayIcon* trayicon;
        Listener* server;
        RichPresence* richPresence;
        ProcessTracker* rbxStudioTracker;

        void initServer();
        void initRichPresence();
        void initSettings();
        void initRbxStudioTracker();

        void updateRobloxStudioStatus(const bool status);

        QString getLoginLaunchLnkPath();

        static QFileInfo getExecutableInfo();

        private slots:
            void loadParams(QJsonObject params);
};
#endif // MAINWINDOW_H

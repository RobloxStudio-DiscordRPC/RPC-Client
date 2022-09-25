#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * \def __VERSION__
 * \brief App version
 */

#include <QMainWindow>
#include <QProcess>
#include <components/listener.h>
#include <components/systemtrayicon.h>
#include <components/richpresence.h>
#include <components/findstatusdisplay.h>
#include <components/settings.h>
#include <components/processtracker.h>
#include <components/updater.h>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QCloseEvent>

#ifndef safedelete
#define safedelete(x) { \
    if ((x)!=NULL) { \
        delete (x); \
        (x)=NULL; \
    } \
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(
        bool startOnLogin
        READ startOnLogin
        WRITE setStartOnLogin
        NOTIFY startOnLoginChanged
    )

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
        GitHubUpdater* updater;

        void initServer();
        void initRichPresence();
        void initSettings();
        void initRbxStudioTracker();
        void initUpdater();

        void updateRobloxStudioStatus(const bool status);

        QString getLoginLaunchLnkPath();

        static QFileInfo getExecutableInfo();
};
#endif // MAINWINDOW_H

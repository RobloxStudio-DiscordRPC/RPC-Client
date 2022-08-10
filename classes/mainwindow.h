#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <classes/listener.h>
#include <classes/systemtrayicon.h>
#include <classes/richpresence.h>

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
    #include <tchar.h>
#endif

#define safedelete(x) {if ((x)!=NULL) {delete (x); (x)=NULL;}}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        bool rbxStudioFound;
        bool refreshRbxStudio();

    private:
        Ui::MainWindow *ui;

        SystemTrayIcon* trayicon;
        Listener* server;
        RichPresence* richPresence;

        void initServer();
        void initRichPresence();

        bool setRbxStudioFound(const bool val);

        private slots:
            void loadParams(QJsonObject params);
};
#endif // MAINWINDOW_H

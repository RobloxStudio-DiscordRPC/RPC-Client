#include "classes/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    MainWindow w;
    w.show();
    //initDiscord();
    return a.exec();
}

#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(): QSystemTrayIcon() {
    app = (QApplication*) QApplication::instance();
    setIcon(app->style()->standardIcon(QStyle::SP_DialogSaveButton));
}

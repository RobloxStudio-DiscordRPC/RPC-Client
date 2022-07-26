#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon() {
    app = (QApplication*) QApplication::instance();
    setIcon(app->style()->standardIcon(QStyle::SP_DialogSaveButton));
}

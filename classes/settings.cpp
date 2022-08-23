#include "settings.h"

Settings::Settings(QFile *f): QJsonDocument() {
    if (!f->exists()) {
        f->open(QFile::OpenModeFlag::WriteOnly);
        f->write("{}");
        f->close();
    }

    setFile(f);
    fillAbout();
}

void Settings::setFile(QFile* json) {
    json->open(QFile::OpenModeFlag::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(
        json->readAll().toStdString().c_str()
    ));
    json->close();

    setObject(doc.object());

    file = json;
}

Settings::~Settings() {
    write();
    delete file;
}

void Settings::fillAbout() {
    QJsonObject obj = object();

    QJsonObject about;
    if (!obj.contains("_ABOUT_CONFIG")) {
        about = QJsonObject();
    } else {
        about = obj.value("_ABOUT_CONFIG").toObject();
    }

    about.insert("VERSION", configVersion);
    about.insert("INFO", configInfo);

    obj.insert("_ABOUT_CONFIG", about);
    setObject(obj);
}

void Settings::set(const QString setting, QVariant value) {
    QJsonObject obj = object();
    obj.insert(setting, QJsonValue::fromVariant(value));
    setObject(obj);
}

QVariant Settings::get(const QString setting) {
    return object().value(setting).toVariant();
}

void Settings::write() {
    file->open(QFile::OpenModeFlag::WriteOnly);
    file->write(toJson());
    file->close();
}

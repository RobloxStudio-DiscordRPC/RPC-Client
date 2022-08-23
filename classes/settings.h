#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>

class Settings : public QObject, public QJsonDocument {
    Q_OBJECT

    public:
        Settings(QFile* f);
        ~Settings();
        QFile* file;

        static inline int configVersion  = 1;
        static inline QString configInfo =
            "Saved settings for Roblox Studio Discord RPC.";

        void setFile(QFile *json);
        void fillAbout();

        void set(const QString setting, QVariant value);
        QVariant get(const QString setting);

        public slots:
            void write();
};

#endif // SETTINGS_H

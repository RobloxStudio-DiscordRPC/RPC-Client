#ifndef UPDATER_H
#define UPDATER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QFile>
#include <QDir>
#include <QProcess>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QTimer>
#include <QEventLoop>

#include <functional>

#ifdef _WIN32
    #include <shldisp.h>
#pragma comment (lib, "Ole32.Lib")
#pragma comment (lib, "OleAut32.lib")
#endif


struct RepoMetadata {
    QString owner = "";
    QString name  = "";
};

class GitHubUpdater : public QNetworkAccessManager {
    Q_OBJECT

    public:
        explicit GitHubUpdater(QObject* parent);

        QString getLatestRelease(bool onlyStable = true);

        RepoMetadata repo;

        /*!
         * \brief Wait for passed reply to finish
         * \param reply The reply to wait for
         * \param timeout Wait timeout
         * \return Returns false if timed out, returns true otherwise
         */
        static bool waitForNetReply(
            QNetworkReply* reply,
            int timeout = -1
        );

        QFile downloadVersion(
            QString version,
            std::function<void(int,int)> progressCallback = NULL
        );

        static bool unzip(QFile* zip);

    private:
};

#endif // UPDATER_H

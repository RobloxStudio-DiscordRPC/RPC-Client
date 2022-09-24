#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <string>
#include <httplib.h>

#include <QJsonDocument>
#include <QJsonObject>

/**
 * \brief GitHub personal access token
 */
#define GH_TOKEN "Bearer {}"

using namespace httplib;

class GitHubUpdater : public QObject, Client {
    Q_OBJECT

    public:
        explicit GitHubUpdater(
            QObject* parent,
            QString  repoOwner,
            QString  repoName
        );

        QString releasePath;
        void check();

    private:
        static const inline Headers reqHeaders = {
            {"Accept", "application/vnd.github+json"},
            {"Authorization", GH_TOKEN}
        };
};

#endif // UPDATER_H

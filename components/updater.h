#ifndef UPDATER_H
#define UPDATER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QApplication>

#pragma region TOKEN {
/**
 * \brief GitHub app access token
 * \link https://github.com/apps/updater-cpp
 */
#define GH_TOKEN "Bearer eyJhbGciOiJSUzI1NiIsImN0eSI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwiaWF0IjoxNjAzMzc2MDExLCJpc3MiOjI0MTM4Mn0.p3u0Sa5LnnXeawxAqyJfW_vcpSZaraUHpBnOX9y095WgOFWWOK9zv9_GCvqXtSpuUhT-fzMIscGnKpaFdYaBGBxyC335GpHfxTgkMFl2TTkWTHXAxxjABrmH4bLckWySN12rjrsXRAgdMhI8x7lJLp3n2zQ_3Iv_nlU6dy8fbXMBRvEGHU5nIGLBV99yx18F3w8WO4CXEv-iPQVIkwua4BKWdYEYSh9QdRmyh05X8nsnkmJiCNbn2xF0O79-WRP0IeeDmjMa6_pHICqetS7NQ7YLut2m1hk_KFQ-eBBuuS0kGsJPPy56rUkFQIaxQvaakTtyZKOlT82s9hXN4HSllg"
#pragma endregion}

class GitHubUpdater : public QNetworkAccessManager {
    Q_OBJECT

    public:
        explicit GitHubUpdater(
            QObject* parent,
            QString  repoOwner,
            QString  repoName
        );

        QUrl releaseUrl;
        void check();

    private:
        /*
        static const inline Headers reqHeaders = {
            {"Accept", "application/vnd.github+json"},
            {"Authorization", GH_TOKEN}
        };
        */
};

#endif // UPDATER_H

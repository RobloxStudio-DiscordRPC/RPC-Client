#include "updater.h"

GitHubUpdater::GitHubUpdater(QObject* parent): QNetworkAccessManager(parent) {}

QString GitHubUpdater::getLatestRelease(bool onlyStable) {
    QString releasesUrl("https://api.github.com/repos/%1/%2/releases");
    releasesUrl = releasesUrl.arg(repo.owner, repo.name);

    QNetworkRequest request(QUrl(releasesUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = get(request);

    if (!waitForNetReply(reply, 5000)) return;

    QByteArray content = reply->readAll();
    if (content.size()==0) return;

    QJsonDocument json = QJsonDocument::fromJson(content);
    QJsonArray releases = json.array();
    if (releases.empty()) return;

    QJsonObject release;
    for (const QJsonValue &releaseVal : releases) {
        release = releaseVal.toObject();
        if (release["prerelease"].toBool() && onlyStable) continue;
        break;
    }

    if (release.empty()) return;

    return release["tag_name"].toString();
}

bool GitHubUpdater::waitForNetReply(QNetworkReply* reply, int timeout) {
    QEventLoop loop;

    connect(
        reply, &QNetworkReply::finished,
        &loop, &QEventLoop::quit
    );

    if (timeout>0) QTimer::singleShot(timeout, &loop, &QEventLoop::quit);

    loop.exec();

    return reply->isFinished();
}

QTemporaryFile GitHubUpdater::downloadVersion(QString version,std::function<void(int,int)> progressCallback) {
    if (version.startsWith('v')) version = version.remove(0,1);
    if (version.toStdString().c_str() == __VERSION__) return;

    QString releasesUrl("https://github.com/%1/%2/releases/download/v%3/RobloxStudio-DiscordRPC-Win64.zip");
    releasesUrl = releasesUrl.arg(repo.owner, repo.name, version);

    QTemporaryFile downloaded(QString("APP_%1").arg(version.replace('.','-')), this);
    downloaded.open(QFile::WriteOnly);

    QNetworkRequest request(QUrl(releasesUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
    QNetworkReply* reply = get(request);

    connect(reply, &QNetworkReply::downloadProgress, progressCallback);
    connect(reply, &QNetworkReply::readyRead, [reply](){downloaded.write(reply->readAll());});

    if (!waitForNetReply(reply, 5000)) return;

    downloaded.close();

    return downloaded;
}

void GitHubUpdater::unzip(QFile* zip) {
    #ifdef _WIN32
        QProcess *process = new QProcess(this);
        QString pwsh("powershell.exe");

        QString cmd(R"(-command "Expand-Archive -Path %1 -DestinationPath %2")");
        cmd = cmd.arg(zip->fileName());

        QStringList parameters{cmd};
        process->start(cmd, parameters);
    #endif
}

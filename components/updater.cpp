#include "updater.h"

GitHubUpdater::GitHubUpdater(QObject* parent): QNetworkAccessManager(parent) {}

QString GitHubUpdater::getLatestRelease(bool onlyStable) {
    QString releasesPath("https://api.github.com/repos/%1/%2/releases");
    releasesPath = releasesPath.arg(repo.owner, repo.name);

    QUrl releasesUrl(releasesPath);

    QNetworkRequest request(releasesUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = get(request);

    if (!waitForNetReply(reply, 5000)) return NULL;

    QByteArray content = reply->readAll();
    if (content.size()==0) return NULL;

    QJsonDocument json = QJsonDocument::fromJson(content);
    QJsonArray releases = json.array();
    if (releases.empty()) return NULL;

    QJsonObject release;
    for (const QJsonValue &releaseVal : releases) {
        release = releaseVal.toObject();
        if (release["prerelease"].toBool() && onlyStable) continue;
        break;
    }

    if (release.empty()) return NULL;

    QString version = release["tag_name"].toString();
    if (version.startsWith('v')) version = version.remove(0,1);

    return version;
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

QFile GitHubUpdater::downloadVersion(QString version,std::function<void(int,int)> progressCallback) {
    if (version.startsWith('v')) version = version.remove(0,1);
    if (version.toStdString().c_str() == __VERSION__) return QFile();

    QString releasesPath("https://github.com/%1/%2/releases/download/v%3/RobloxStudio-DiscordRPC-Win64.zip");
    releasesPath = releasesPath.arg(repo.owner, repo.name, version);
    QUrl releasesUrl(releasesPath);

    QString downloadPath = (
        QDir::tempPath() +
        QDir::separator() +
        QString("APP_%1").arg(version.replace('.','-')) +
        ".zip"
    );
    QFile downloaded(downloadPath, this);
    downloaded.open(QFile::WriteOnly);

    QNetworkRequest request(releasesUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
    QNetworkReply* reply = get(request);


    if (progressCallback!=NULL) connect(reply, &QNetworkReply::downloadProgress, this, progressCallback);
    connect(
        reply, &QNetworkReply::readyRead,
        this, [reply, &downloaded, &progressCallback]() {
            downloaded.write(reply->readAll());
        }
    );

    if (!waitForNetReply(reply)) return QFile();

    downloaded.close();

    return QFile(downloaded.fileName());
}

void GitHubUpdater::unzip(QFile* zip) {
    if (!zip->fileName().endsWith(".zip")) return;

    #ifdef _WIN32
        QProcess *process = new QProcess(zip);
        QString pwsh("powershell.exe");

        QString cmd(R"(-command "Expand-Archive -Path %1 -DestinationPath %2")");
        cmd = cmd.arg(zip->fileName(), QDir::tempPath());

        QStringList parameters{cmd};
        process->start(pwsh, parameters);
    #endif
}

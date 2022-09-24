#include "updater.h"

GitHubUpdater::GitHubUpdater(
    QObject* parent,
    QString  repoOwner,
    QString  repoName
): QObject(parent), Client("https://api.github.com") {

    releasePath = QString("/repos/%1/%2/releases/latest").arg(repoOwner, repoName);

}

void GitHubUpdater::check() {
    Response res = Get(releasePath.toStdString(), reqHeaders).value();
    QJsonObject json = QJsonDocument::fromJson(QByteArray(res.body.data())).object();

    if (!json["prerelease"].toBool()) return;
    qDebug() << json["tag_name"].toString();
}

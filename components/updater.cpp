#include "updater.h"

GitHubUpdater::GitHubUpdater(
    QObject* parent,
    QString  repoOwner,
    QString  repoName
): QNetworkAccessManager(parent) {

    releaseUrl = QUrl(QString("https://api.github.com/repos/%1/%2/releases/latest").arg(repoOwner, repoName));

}

void GitHubUpdater::check() {
    QNetworkRequest request(releaseUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = get(request);

    bool timeout=false;

    while(!timeout){
        QApplication::instance()->processEvents();
        if(reply->isFinished()) break;
    }

    if(reply->isFinished()){
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        qDebug() << json[0]["name"].toString();
    }else{
        qDebug() << "Timeout";
    }
}

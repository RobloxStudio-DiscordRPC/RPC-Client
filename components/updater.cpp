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

bool GitHubUpdater::unzip(QFile* zip) {
    if (!zip->fileName().endsWith(".zip")) return false;

    QFileInfo zipInfo(*zip);

    QDir destDir(zipInfo.absolutePath() + '\\'+ zipInfo.baseName());
    if (!destDir.exists()) zipInfo.absoluteDir().mkdir(zipInfo.baseName());
    else {
        destDir.setNameFilters(QStringList() << "*");
        destDir.setFilter(QDir::AllEntries);
        foreach(QString child, destDir.entryList()) destDir.remove(child);
    }

    #ifdef _WIN32

        BSTR source = SysAllocString(QDir::toNativeSeparators(zipInfo.absoluteFilePath()).toStdWString().c_str());
        BSTR dest   = SysAllocString(QDir::toNativeSeparators(destDir.absolutePath())    .toStdWString().c_str());

        //WORD     strlen = 0;
        HRESULT     hResult;
        IShellDispatch *pISD;
        Folder  *pToFolder = NULL;
        Folder  *pFromFolder = NULL;
        FolderItems *pFolderItems = NULL;
        //FolderItem *pItem = NULL;

        VARIANT   vDir, vFile, vOpt;
        //BSTR  strptr1, strptr2;
        CoInitialize(NULL);

        bool bReturn = false;

        hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellDispatch, (void **)&pISD);

        if (FAILED(hResult)) return bReturn;

        VariantInit(&vOpt);
        vOpt.vt = VT_I4;
        vOpt.lVal = 1024+512+256+16+8+4;     // Do not display a progress dialog box ~ This will not work properly!

        VariantInit(&vFile);
        vFile.vt = VT_BSTR;
        vFile.bstrVal = source;
        hResult = pISD->NameSpace(vFile, &pFromFolder);

        VariantInit(&vDir);
        vDir.vt = VT_BSTR;
        vDir.bstrVal = dest;

        hResult |= pISD->NameSpace(vDir, &pToFolder);

        if(S_OK == hResult) {
            hResult = pFromFolder->Items(&pFolderItems);
            if(SUCCEEDED(hResult)) {
                long lCount = 0;
                pFolderItems->get_Count(&lCount);
                IDispatch* pDispatch = NULL;
                pFolderItems->QueryInterface(IID_IDispatch,(void**)&pDispatch);
                VARIANT vtDispatch;
                VariantInit(&vtDispatch);
                vtDispatch.vt = VT_DISPATCH;
                vtDispatch.pdispVal = pDispatch;

                //cout << "Extracting files ...\n";
                #ifdef QT_DEBUG
                    try {
                        hResult = pToFolder->CopyHere(vtDispatch,vOpt);
                        if (hResult != S_OK) return false;
                    }catch(int){}
                #else
                    hResult = pToFolder->CopyHere(vtDispatch,vOpt);
                    if (hResult != S_OK) return false;
                #endif



                //Cross check and wait until all files are zipped!
                FolderItems* pToFolderItems;
                hResult = pToFolder->Items(&pToFolderItems);

                if(S_OK == hResult) {
                    long lCount2 = 0;

                    hResult = pToFolderItems->get_Count(&lCount2);
                    if(S_OK != hResult) {
                        pFolderItems->Release();
                        pToFolderItems->Release();
                        SysFreeString(source);
                        SysFreeString(dest);
                        pISD->Release();
                        CoUninitialize();
                        return false;
                    }
                    //Use this code in a loop if you want to cross-check the items unzipped.
                    /*if(lCount2 != lCount)
                    {
                    pFolderItems->Release();
                    pToFolderItems->Release();
                    SysFreeString(source);
                    SysFreeString(strptr2);
                    pISD->Release();
                    CoUninitialize();
                    return false;
                    }*/

                    bReturn = true;
                }

                pFolderItems->Release();
                pToFolderItems->Release();
            }

            pToFolder->Release();
            pFromFolder->Release();
        }

        //cout << "Over!\n";
        SysFreeString(source);
        SysFreeString(dest);
        pISD->Release();

        CoUninitialize();

        return bReturn;
    #endif
}

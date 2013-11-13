#ifndef SELFUPDATER_H
#define SELFUPDATER_H

#include <QDialog>
#include <QNetworkAccessManager>

namespace Ui {
class SelfUpdater;
}

class SelfUpdater : public QDialog
{
    Q_OBJECT

public:
    explicit SelfUpdater(bool silent, QWidget *parent);
    ~SelfUpdater();

    int execUpdate();
    void silentCheck();

    void closeEvent(QCloseEvent* event);

public slots:
    void downloadVersionFinished(QNetworkReply* reply);
    void downloadNewVersionFinished(QNetworkReply* reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::SelfUpdater* ui;
    QNetworkAccessManager selfUpdateNetworkManager;

    bool allowClose;
    bool silent;
};

#endif // SELFUPDATER_H

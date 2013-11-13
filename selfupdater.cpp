#include "selfupdater.h"
#include "ui_selfupdater.h"
#include "mainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QNetworkReply>
#include <QCloseEvent>

SelfUpdater::SelfUpdater(bool silent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelfUpdater), selfUpdateNetworkManager(this) {

    ui->setupUi(this);

    this->silent = silent;

    allowClose = true;
}

SelfUpdater::~SelfUpdater() {
    delete ui;
}

int SelfUpdater::execUpdate() {
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(1);
    ui->label->setText("Checking for updates...");

    connect(&selfUpdateNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadVersionFinished(QNetworkReply*)));

    QNetworkRequest request(MainWindow::selfUpdateUrl);

    selfUpdateNetworkManager.get(request);

    return exec();
}

void SelfUpdater::silentCheck() {
    connect(&selfUpdateNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadVersionFinished(QNetworkReply*)));

    QNetworkRequest request(MainWindow::selfUpdateUrl);

    selfUpdateNetworkManager.get(request);
    //reply->
}

void SelfUpdater::closeEvent(QCloseEvent* event) {
    if (!allowClose)
        event->ignore();
    else
        QDialog::closeEvent(event);
}

void SelfUpdater::downloadVersionFinished(QNetworkReply* reply) {
    ui->progressBar->setValue(1);

    disconnect(&selfUpdateNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadVersionFinished(QNetworkReply*)));

    if (reply->error() != QNetworkReply::NoError) {
        if (!silent)
            QMessageBox::warning(this, "ERROR", reply->errorString());

        done(0);

        return;
    }

    QString data = reply->readAll();

    if (!data.contains(";")) {
        if (!silent)
            QMessageBox::warning(this, "ERROR", "Could not check for newest version, unknown format!");

        done(0);

        return;
    }

    QStringList list = data.split(";");

    if (list.size() < 2) {
        if (!silent)
            QMessageBox::warning(this, "ERROR", "Could not check for newest version, unknown format!");

        done(0);

        return;
    }

    if (list.at(1).toDouble() <= MainWindow::version.toDouble()) {
        if (!silent)
            QMessageBox::information(this, "Version", "Launcher already on latest version");

        //emit finished(0);

        done(0);

        return;
    } else {
        if (silent) {
            if (QMessageBox::question(this, "Update", "A new version of the launchpad has been found, do you want to update?") != QMessageBox::Yes)
                return;
        }
    }

    ui->label->setText("Downloading new version...");

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);

    connect(&selfUpdateNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadNewVersionFinished(QNetworkReply*)));

    QNetworkReply* newReply = selfUpdateNetworkManager.get(QNetworkRequest(list.at(0)));
    connect(newReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

    if (silent)
        exec();
}

void SelfUpdater::downloadNewVersionFinished(QNetworkReply* reply) {
    ui->progressBar->setValue(ui->progressBar->maximum());

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "ERROR", reply->errorString());

        done(0);

        return;
    }

    qDebug() << "finished downloading new version";

    int bufferSize = 8192 * 2;
    char* buffer = (char*) malloc(bufferSize);

    QFile fileObject("setup.exe");
    //fileObject.set

    if (!fileObject.open(QIODevice::WriteOnly)) {
        if (!silent)
            QMessageBox::warning(this, "ERROR", "Could not open to write downloaded file to disk!");

        free(buffer);

        done(0);

        return;
    }

    int read = 0;
    while ((read = reply->read(buffer, bufferSize)) > 0) {
        if (fileObject.write(buffer, read) == -1) {
            if (!silent)
                QMessageBox::warning(this, "ERROR", "Could not write downloaded file to disk!");

            free(buffer);

            done(0);

            return;
        }
    }

    fileObject.close();
    free(buffer);

    bool startRes = QProcess::startDetached("setup.exe");

    if (startRes)
        QApplication::quit();

    done(1);
}

void SelfUpdater::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    //qDebug() << " progress " << ui->progressBar->value() << " " << bytesReceived << " " << bytesTotal;

    if (bytesTotal <= 0)
        return;

    if (ui->progressBar->value() == 0) {
        ui->progressBar->setMaximum(bytesTotal);
    }

    ui->progressBar->setValue(bytesReceived);
}

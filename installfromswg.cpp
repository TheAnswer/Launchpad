#include "installfromswg.h"
#include "ui_installfromswg.h"
#include <QSettings>
#include <QDir>
#include "mainwindow.h"
#include <QTimer>
#include <QFutureWatcher>
#include <QPalette>
#include <QMessageBox>
#include <QFileDialog>

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#endif

InstallFromSWG::InstallFromSWG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstallFromSWG) {
    ui->setupUi(this);

    cancelThreads = false;

    connect(&copyWatcher, SIGNAL(finished()), this, SLOT(copyFinished()));

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(0);
    //connect(this, SIGNAL())

    connect(this, SIGNAL(fileCopiedSignal(QString,bool)), this, SLOT(fileCopied(QString,bool)));
}

InstallFromSWG::~InstallFromSWG() {
    delete ui;
}

void InstallFromSWG::copyFinished() {
    int result = copyWatcher.result();

    ui->progressBar->setValue(ui->progressBar->maximum());

    if (result == 0)
        ui->label->setText("Installation finished.");
    else
        ui->label->setText("Installation failed.");

    qDebug() << "copy finished with result " << result;

    done(result);
}

void InstallFromSWG::closeEvent(QCloseEvent* event) {
    qDebug() << "close event";

    cancelThreads = true;

    if (copyWatcher.isRunning()) {
        copyWatcher.cancel();
        copyWatcher.waitForFinished();
    }

    done(2);

    QDialog::closeEvent(event);
}

int InstallFromSWG::copyFiles() {
    QVector<QPair<QString, qint64> > requiredFiles = MainWindow::getRequiredFiles();

    for (int i = 0; i < requiredFiles.size() && !cancelThreads; ++i) {
        const QPair<QString, qint64> file = requiredFiles.at(i);

        //if (QDir(file))

        if (file.first.contains("/")) {
            QString dir = emuFolder + file.first.mid(0, file.first.lastIndexOf("/"));

            QDir(dir).mkpath(".");
        }

        bool result = QFile::copy(swgfolder + "\\" + file.first, emuFolder + file.first);

        //bool result = true;
        //QTimer::singleShot(0, this, SLOT(fileCopied(file.first, result)));
        emit fileCopiedSignal(file.first, result);
/*
        if (!result)
            return 1;*/
    }

    return 0;
}

void InstallFromSWG::fileCopied(const QString& file, bool success) {
    if (success) {
        ui->label->setText(file + " successfully installed");
    }

    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

int InstallFromSWG::checkSWGFolder() {
    QDir dir(swgfolder);

    if (!dir.exists())
        return 1;

    QStringList filesToCheck;
    filesToCheck << "bottom.tre" << "data_animation_00.tre" << "data_texture_04.tre";

    for (int i = 0; i < filesToCheck.size(); ++i) {
        if (!QFile(swgfolder + "\\" + filesToCheck.at(i)).exists())
            return 2;
    }

    return 0;
}

int InstallFromSWG::installFiles() {
    QSettings settings;

    QMessageBox::information(this, "SWGEmu", "Please choose a valid Star Wars Galaxies installation");

    swgfolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "/home",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);

    int validFolder = checkSWGFolder();

    if (validFolder != 0) {
        QMessageBox::warning(this, "Folder", "The folder you selected isnt a valid Star Wars Galaxies installation!");

        return 1;
    }

    QMessageBox::information(this, "SWGEmu", "Please choose where you want to install SWGEmu");

    emuFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!QDir(emuFolder).exists() || emuFolder.isEmpty()) {
        QMessageBox::warning(this, "Folder", "The swgemu folder you selected isn't a valid directory");

        return 1;
    }

    //qDebug() << emuFolder;
    emuFolder = emuFolder + "/SWGEmu/";

    if (QDir(emuFolder).exists()) {
        if (QMessageBox::question(this, "Warning", "SWGEmu folder already exists, do you want to overwrite?") != QMessageBox::Yes)
            return 3;
    } else {
        if (!QDir(emuFolder).mkpath(".")) {
            QMessageBox::warning(this, "ERROR", "Could not create the swgemu folder!");
            return 4;
        }
    }

    QVector<QPair<QString, qint64> > requiredFiles = MainWindow::getRequiredFiles();

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(requiredFiles.size());

    QFuture<int> future = QtConcurrent::run(this, &InstallFromSWG::copyFiles);
    copyWatcher.setFuture(future);

    return exec();
}

#ifndef INSTALLFROMSWG_H
#define INSTALLFROMSWG_H

#include <QDialog>
#include <QString>
#include <QFutureWatcher>

namespace Ui {
class InstallFromSWG;
}

class InstallFromSWG : public QDialog {
    Q_OBJECT

public:
    explicit InstallFromSWG(QWidget *parent = 0);
    ~InstallFromSWG();

public slots:
    int installFiles();
    int checkSWGFolder();
    int copyFiles();
    void fileCopied(const QString& file, bool success);
    void copyFinished();

public:
    void closeEvent(QCloseEvent *event);
    QString getEmuFolder() {
        return emuFolder;
    }

signals:
    void fileCopiedSignal(QString, bool);

private:
    Ui::InstallFromSWG* ui;
    QFutureWatcher<int> copyWatcher;
    QString swgfolder;
    QString emuFolder;

    volatile bool cancelThreads;
};

#endif // INSTALLFROMSWG_H

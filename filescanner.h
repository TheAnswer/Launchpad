#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <QObject>

class MainWindow;

class FileScanner : public QObject {
    Q_OBJECT

    MainWindow* mainWindow;
public:
    FileScanner(MainWindow* main);

    void fullScanMultiThreaded(bool);
    int fullScanSingleThreaded(bool);

    int loadAndBasicCheckFiles(QString swgFolder);

    void fullScanFile(const QString& file, const QString& name, qint64, const QString &md5);


    static bool checkSwgFolder(const QString& dir);
    static bool isARequiredFile(const QString& name);

signals:
    void fullScannedFile(QString, bool);
    void addFileToDownload(QString);
    void requiredFileExists(QString);

};

#endif // FILESCANNER_H

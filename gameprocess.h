#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

#include <QDialog>
#include <QProcess>

class ConfigParser;

namespace Ui {
class GameProcess;
}

class WinDebugMonitor;

class GameProcess : public QDialog
{
    Q_OBJECT

public:
    explicit GameProcess(ConfigParser* parser, QWidget *parent = 0);
    ~GameProcess();

    bool start(const QString& folder, const QString& executable, const QStringList& arguments);

public slots:
    void startError(QProcess::ProcessError err);
    void outputChannelDataReady();
    void outputDebugString(int processId, QString str);
    void outputDebugString(QString str);
    void finished(int, QProcess::ExitStatus);
    void started();
    void dumpConfigValues();
    void killProcess();
    void saveLogToFile();
    void clearOutputLogScreen();

signals:
    void processFinished(GameProcess* process, int exitCode);

private:
    Ui::GameProcess *ui;
    QProcess* process;
    ConfigParser* configParser;

#ifdef Q_OS_WIN32
public:
    static WinDebugMonitor* debugMonitor;
#endif
};

#endif // GAMEPROCESS_H

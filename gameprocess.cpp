#define NOMINMAX

#include "gameprocess.h"
#include "ui_gameprocess.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include "windebugmonitor.h"
#include "configparser.h"
#include <QScrollBar>
#include <QDateTime>

WinDebugMonitor* GameProcess::debugMonitor = NULL;

GameProcess::GameProcess(ConfigParser* config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameProcess) {
    ui->setupUi(this);

    process = NULL;
    configParser = config;
    connect(ui->pushButton_config, SIGNAL(clicked()), this, SLOT(dumpConfigValues()));
    connect(ui->pushButton_kill, SIGNAL(clicked()), this, SLOT(killProcess()));
}

GameProcess::~GameProcess() {
    delete ui;
    delete configParser;
}

bool GameProcess::start(const QString& folder, const QString& executable, const QStringList &arguments) {
    process = new QProcess();
    QSettings settings;

    process->setWorkingDirectory(folder);

    if (debugMonitor == NULL) {
        debugMonitor = new WinDebugMonitor();
    }

    if (!settings.value("close_after_start", false).toBool() && !debugMonitor->IsInitialized() && settings.value("capture_debug_output", false).toBool()) {
        int ret = debugMonitor->Initialize();

        if (ret != 0)
            ui->textBrowser->append("Warning: Debug Monitor could not be initialized, if you want the debug output enabled please start the launchpad with administrator privileges");
    } else if (!settings.value("capture_debug_output", false).toBool()) {
        if (debugMonitor->IsInitialized()) {
            ui->textBrowser->append("Please restart the launchpad to disable the debug output");
        } else
            ui->textBrowser->append("Capture debug output is disabled");
    }

    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(startError(QProcess::ProcessError)));
    connect(debugMonitor, SIGNAL(outputDebugString(int, QString)), this, SLOT(outputDebugString(int, QString)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(started()), this, SLOT(started()));

    QStringList env = process->environment();
    env.append("SWGCLIENT_MEMORY_SIZE_MB=4096");

    qDebug() << "env:" << env;
    process->setEnvironment(env);
    process->start(folder + "\\" + executable, arguments);
    return true;
}

void GameProcess::killProcess() {
    if (!process)
        return;

    if (QMessageBox::question(this, "Warning", "Are you sure you want to attempt terminate this process?") != QMessageBox::Yes)
        return;

    //process->kill();
    process->terminate();
}

void GameProcess::dumpConfigValues() {
    QMap<QString, QVector<ConfigValue> >* values = configParser->getAllValues();

    QMapIterator<QString, QVector<ConfigValue> > it(*values);

    ui->textBrowser->append("------------------------------------------------------------------------------------------------------------------------------------");
    ui->textBrowser->append("Starting the config value dump");

    while (it.hasNext()) {
        it.next();

        QString className = it.key();

        ui->textBrowser->append("[" + className + "]");
        QVector<ConfigValue> configValues = it.value();

        for (int i = 0; i < configValues.size(); ++i) {
            const ConfigValue val = configValues.at(i);

            ui->textBrowser->append("\t" + val.variableName + " = " + val.variableValue + " \t\tdefined in: " + val.fileName);
        }

        ui->textBrowser->append("\n");
    }
    ui->textBrowser->append("Config dump end");
    ui->textBrowser->append("------------------------------------------------------------------------------------------------------------------------------------");
    //ui->textBrowser->

    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void GameProcess::started() {
    //qDebug() << "proces started with state: " << process->state();

    if (process && process->state() != QProcess::Running)
        return;

    QSettings settings;
    if (settings.value("close_after_start", false) == true) {
        QApplication::quit();
    }
}

void GameProcess::outputDebugString(QString str) {
    ui->textBrowser->append(str);
}

void GameProcess::outputDebugString(int processId, QString str) {
    //QDateTime currentStamp = ;

    QString date = QDateTime::currentDateTime().toString("hh:mm:ss");
    if (process && process->pid()->dwProcessId == processId) {
        ui->textBrowser->append(date + ": " + str);
    }
}

void GameProcess::finished(int exitCode, QProcess::ExitStatus ) {
    process = NULL;

    ui->textBrowser->append("Process exited with exit code " + QString::number(exitCode));

    emit processFinished(this, exitCode);
}

void GameProcess::outputChannelDataReady() {
    qDebug() << "data ready emitted";
    //qDebug() << process->readAllStandardOutput();
}

void GameProcess::startError(QProcess::ProcessError err) {
    qDebug() << "start swg error:" << err;

    QString errorMessage;

    switch (err) {
    case QProcess::FailedToStart:
        errorMessage = "File not found!";
        break;
    case QProcess::Crashed:
        errorMessage = "Process crashed!";
        break;
    default:
        errorMessage = "Unknown error!";
        break;
    }

    //QMessageBox::critical( this, "ERROR", "Could not launch client! " + errorMessage);

    ui->textBrowser->append(errorMessage);
}

void GameProcess::clearOutputLogScreen() {
    ui->textBrowser->clear();
}

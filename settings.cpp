#include "settings.h"
#include "ui_settings.h"
#include <QSettings>
#include <QFileDialog>
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings) {
    ui->setupUi(this);

    QSettings settings;

    ui->lineEdit_swgfolder->setText(settings.value("swg_folder").toString());
    ui->checkBox_close->setChecked(settings.value("close_after_start", false).toBool());
    ui->checkBox_minimize->setChecked(settings.value("minimize_after_start", false).toBool());
    ui->checkBox_debug->setChecked(settings.value("capture_debug_output", false).toBool());
    ui->checkBox_multithreaded->setChecked(settings.value("multi_threaded_full_scan", false).toBool());

    connect(ui->pushButton_SelectFolder, SIGNAL(clicked()), this, SLOT(selectFolder()));
    //connect(ui->lineEdit_swgfolder, SIGNAL(textChanged(QString)), this, SLOT(folderChanged(QString)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(updateAllOptions()));
    connect(ui->buttonBox, SIGNAL(accepted()), (MainWindow*)parent, SLOT(startLoadBasicCheck()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(restoreOptions()));
    connect(ui->checkBox_close, SIGNAL(toggled(bool)), this, SLOT(closeAfterStartChanged(bool)));
    connect(ui->checkBox_minimize, SIGNAL(toggled(bool)), this, SLOT(minimizeToTrayAfterStartChanged(bool)));
    //connect(ui->)
}

Settings::~Settings() {
    delete ui;
    ui = NULL;
}

void Settings::selectFolder() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->lineEdit_swgfolder->setText(dir);
}

void Settings::restoreMimizeAfterStart() {
    QSettings settings;
    ui->checkBox_minimize->setChecked(settings.value("minimize_after_start", false).toBool());
}

void Settings::updateAllOptions() {
    updateCloseAfterStartSetting();
    updateFolderSetting();
    updateMinimizeAfterStart();
    updateCaptureDebugSetting();
    updateMultiThreadedFullScan();
}

void Settings::updateMultiThreadedFullScan() {
    QSettings settings;
    settings.setValue("multi_threaded_full_scan", ui->checkBox_multithreaded->isChecked());
}

void Settings::updateCaptureDebugSetting() {
    QSettings settings;
    settings.setValue("capture_debug_output", ui->checkBox_debug->isChecked());
}

void Settings::updateMinimizeAfterStart() {
    QSettings settings;
    settings.setValue("minimize_after_start", ui->checkBox_minimize->isChecked());
}

void Settings::closeAfterStartChanged(bool state) {
    if (state && ui->checkBox_minimize->isChecked())
        ui->checkBox_minimize->setChecked(false);
}

void Settings::minimizeToTrayAfterStartChanged(bool state) {
    if (state && ui->checkBox_close->isCheckable())
        ui->checkBox_close->setChecked(false);
}

void Settings::updateCloseAfterStartSetting() {
    QSettings settings;
    settings.setValue("close_after_start", ui->checkBox_close->isChecked());
}

void Settings::updateFolderSetting() {
    QString newFolder = ui->lineEdit_swgfolder->text();

    QSettings settings;
    settings.setValue("swg_folder", newFolder);
}

void Settings::restoreOptions() {
    restoreFolder();
    restoreCloseAfterStart();
    restoreMimizeAfterStart();
    restoreCaptureDebug();
    restoreMultiThreadedFullScan();
}

void Settings::restoreCaptureDebug() {
    QSettings settings;
    ui->checkBox_debug->setChecked(settings.value("capture_debug_output", false).toBool());
}

void Settings::restoreCloseAfterStart() {
    QSettings settings;
    ui->checkBox_close->setChecked(settings.value("close_after_start", false).toBool());
}

void Settings::restoreMultiThreadedFullScan() {
    QSettings settings;
    ui->checkBox_multithreaded->setChecked(settings.value("multi_threaded_full_scan", false).toBool());
}

void Settings::restoreFolder() {
    QSettings settings;
    QString folder = settings.value("swg_folder").toString();

    ui->lineEdit_swgfolder->setText(folder);
}

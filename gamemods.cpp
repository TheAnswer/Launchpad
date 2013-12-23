#include "gamemods.h"
#include "ui_gamemods.h"
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#define FPS_ADDRESS 0x1153

GameMods::GameMods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameMods) {
    ui->setupUi(this);

    checkMods();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(setMods()));
}

GameMods::~GameMods() {
    delete ui;
}

void GameMods::checkMods() {
    checkMovementMod();
    if (!checkFpsMod())
        QMessageBox::warning(this, "FPS", "It seems that you don't have the original SWGEmu.exe, FPS overrider will be disabled");
}

bool GameMods::checkMovementMod() {
    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    bool val = QFile(swgFolder + "/object/creature/player/base/shared_base_player.iff").exists();

    ui->checkBox_movement->setChecked(val);
    return val;
}

bool GameMods::checkFpsMod() {
    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    QFile executable(swgFolder + "/SWGEmu.exe");

    bool returnVal = false;

    if (executable.open(QIODevice::ReadOnly)) {
        bool result = executable.seek(FPS_ADDRESS);

        if (result) {
            //char buffer[7];
            QByteArray array = executable.read(3);

            //quint32 data = *(quint32*)buffer;

            //qDebug() << array.toHex();

            //qDebug() << array.toULong();

            if (array.size() == 3 && array[0] == (char)0xc7 && array[1] == (char)0x45 && array[2] == (char)0x94) {
                if (executable.seek(FPS_ADDRESS + 3)) {
                    float val;
                    if (executable.read((char*)&val, 4) == 4) {
                        ui->spinBox->setValue(val);

                        returnVal = true;
                    }
                }
            }

        }

    }

    if (!returnVal) {
        ui->spinBox->setEnabled(false);
    }

    return returnVal;
}

void GameMods::setMods() {
    setFpsMod(ui->spinBox->value());
    setMovementMod(ui->checkBox_movement->isChecked());
}

void GameMods::setFpsMod(int fps) {
    if (!ui->spinBox->isEnabled())
        return;

    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    QFile executable(swgFolder + "/SWGEmu.exe");

    bool modResult = false;

    if (executable.open(QIODevice::ReadWrite)) {
        bool result = executable.seek(FPS_ADDRESS);

        if (result) {
            QByteArray array = executable.read(3);

            if (array.size() == 3 && array[0] == (char)0xc7 && array[1] == (char)0x45 && array[2] == (char)0x94) {
                if (executable.seek(FPS_ADDRESS + 3)) {
                    float val;
                    if (executable.read((char*)&val, 4) == 4) {
                        if (val != fps) {
                            executable.seek(FPS_ADDRESS + 3);

                            float newValue = (float) fps;
                            if (executable.write((char*)&newValue, 4) == 4)
                                modResult = true;
                        } else
                            modResult = true;
                    }
                }
            }

        }
    }

    executable.close();

    if (!modResult) {
        QMessageBox::warning(this, "ERROR", "Could not patch SWGEmu.exe to override fps setting!");
    }
}

void GameMods::setMovementMod(bool val) {
    QSettings settings;
    QString swgFolder = settings.value("swg_folder").toString();

    if (val) {
        qDebug() << "settings movement mod";
        QDir dir(swgFolder + "/object/creature/player/base/");

        if (!dir.exists())
            dir.mkpath(".");

        if (QFile(swgFolder + "/object/creature/player/base/shared_base_player.iff").exists())
            return;

        if (QFile::copy(":/files/shared_base_player.iff", swgFolder + "/object/creature/player/base/shared_base_player.iff")) {
            QFile file(swgFolder + "/object/creature/player/base/shared_base_player.iff");
            file.setPermissions(QFile::WriteUser);
        } else {
            QMessageBox::warning(this, "ERROR", "Failed to set the movement mod! Could not copy file");
        }
    } else {
        qDebug() << "removing movement mod";
        if (QFile(swgFolder + "/object/creature/player/base/shared_base_player.iff").exists()) {
            if (!QFile::remove(swgFolder + "/object/creature/player/base/shared_base_player.iff")) {
                QMessageBox::warning(this, "ERROR", "Failed to remove the movement mod! Could not delete file");
            }
        }
        //file.open()
        //file.remove();
    }
}

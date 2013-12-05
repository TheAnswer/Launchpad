#ifndef GAMEMODS_H
#define GAMEMODS_H

#include <QDialog>

namespace Ui {
class GameMods;
}

class GameMods : public QDialog
{
    Q_OBJECT

public:
    explicit GameMods(QWidget *parent = 0);
    ~GameMods();

    void checkMods();

    bool checkMovementMod();
    bool checkFpsMod();

    void setFpsMod(int fps);
    void setMovementMod(bool val);

public slots:
    void setMods();

private:
    Ui::GameMods *ui;
};

#endif // GAMEMODS_H

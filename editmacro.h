#ifndef EDITMACRO_H
#define EDITMACRO_H

#include <QDialog>
#include <QGraphicsScene>

#include "gamemacro.h"

namespace Ui {
class EditMacro;
}

class EditMacro : public QDialog {
    Q_OBJECT

public:
    explicit EditMacro(GameMacro* macro, QWidget *parent = 0);
    ~EditMacro();

    QImage getIconsImage();

protected:

private:
    Ui::EditMacro *ui;
    QGraphicsScene* scene;
    GameMacro* macro;
};

#endif // EDITMACRO_H

#ifndef MACROEDITOR_H
#define MACROEDITOR_H

#include <QDialog>
#include <QImage>
#include <QFile>

#include "gamemacro.h"
#include <QMap>

namespace Ui {
class MacroEditor;
}

class MacroEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MacroEditor(QWidget *parent = 0);
    ~MacroEditor();

public slots:
    void editCurrentSelectedMacro();

protected:
    void loadAccounts();
    void loadMacros(const QString& account);
    QStringList loadMacroNames(QFile& macrosFile);

private:
    Ui::MacroEditor *ui;
    QMap<int, GameMacro*> gameMacros;

};

#endif // MACROEDITOR_H

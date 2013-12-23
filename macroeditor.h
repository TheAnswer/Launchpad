#ifndef MACROEDITOR_H
#define MACROEDITOR_H

#include <QDialog>

namespace Ui {
class MacroEditor;
}

class MacroEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MacroEditor(QWidget *parent = 0);
    ~MacroEditor();

private:
    Ui::MacroEditor *ui;
};

#endif // MACROEDITOR_H

#include "macroeditor.h"
#include "ui_macroeditor.h"

MacroEditor::MacroEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MacroEditor) {
    ui->setupUi(this);
}

MacroEditor::~MacroEditor() {
    delete ui;
}

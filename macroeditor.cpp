#include "macroeditor.h"
#include "ui_macroeditor.h"
#include "editmacro.h"
#include <QSettings>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include "gamemacro.h"

MacroEditor::MacroEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MacroEditor) {
    ui->setupUi(this);

    connect(ui->pushButton_edit, SIGNAL(clicked()), this, SLOT(editCurrentSelectedMacro()));
    connect(ui->comboBox_accounts, SIGNAL(currentTextChanged(QString)), this, SLOT(accountChanged(QString)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(macroDoubleClicked(QListWidgetItem*)));

    loadAccounts();
}

MacroEditor::~MacroEditor() {
    delete ui;
}

void MacroEditor::accountChanged(QString account) {
    ui->listWidget->clear();

    loadMacros(account);
}

void MacroEditor::macroDoubleClicked(QListWidgetItem* item) {
    GameMacro* macro = dynamic_cast<GameMacro*>(item);

    if (macro == NULL)
        return;

    editCurrentSelectedMacro();
}

void MacroEditor::loadAccounts() {
    QSettings settings;
    QString folder = settings.value("swg_folder").toString();

    QDir profilesFolder(folder + "/profiles");

    if (!profilesFolder.exists())
        return;

    QStringList accounts = profilesFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (accounts.size() <= 0)
        return;

    for (int i = 0; i < accounts.size(); ++i) {
        ui->comboBox_accounts->addItem(accounts.at(i));
    }

  //  loadMacros(accounts.at(0));
}

void MacroEditor::loadMacros(const QString& account) {
    QSettings settings;
    QString folder = settings.value("swg_folder").toString();

    QFile file(folder + "/profiles/" + account + "/macros.txt");

    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "loadMacros(account) file.open failed";

        return;
    }

    QStringList macros = loadMacroNames(file);

    QMap<int, GameMacro*>::iterator i;
    for (i = gameMacros.begin(); i != gameMacros.end(); ++i) {
        //cout << i.key() << ": " << i.value() << endl;
        GameMacro* macro = i.value();

        ui->listWidget->addItem(macro);
    }
}

QStringList MacroEditor::loadMacroNames(QFile& macrosFile) {
    QStringList names;

    QTextStream stream(&macrosFile);

    if (!stream.atEnd()) {
        QString version = stream.readLine();

        if (!version.contains("version: 0000")) {
            qDebug() << "invalid macros.txt format";

            return names;
        }
    }

    while (!stream.atEnd()) {
        QString line = stream.readLine();

        GameMacro* macro = new GameMacro();
        //connect(macro, SIGNAL())
        if (!macro->parseMacro(line)) {
            delete macro;
            continue;
        }

        names.append(macro->getName());

        gameMacros.insert(macro->getId(), macro);
    }

    return names;
}

void MacroEditor::editCurrentSelectedMacro() {
    int idx = ui->listWidget->currentRow();

    if (idx < 0)
        return;

    GameMacro* macro = dynamic_cast<GameMacro*>(ui->listWidget->currentItem());

    if (macro != NULL) {
        EditMacro dialog(macro, this);
        dialog.exec();
    }
}


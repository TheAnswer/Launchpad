#include "loginservers.h"
#include "ui_loginservers.h"
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include "addloginserver.h"
#include "mainwindow.h"
#include <QSettings>

QString LoginServers::defaultLoginAddress = "login.swgemu.com";
quint16 LoginServers::defaultLoginPort = 44453;

LoginServers::LoginServers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginServers) {
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addServer()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(removeServer()));
    connect(ui->buttonBox, SIGNAL(accepted()),  (MainWindow*)parent, SLOT(updateLoginServerList()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reloadServers()));
    connect(ui->pushButton_edit, SIGNAL(clicked()), this, SLOT(editServer()));

    reloadServers();
}

LoginServers::~LoginServers() {
    delete ui;
}

void LoginServers::reloadServers() {
    ui->listWidget->clear();

    QSettings settings;

    int size = settings.beginReadArray("login_servers");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString host = settings.value("host").toString();
        unsigned short port = (unsigned short) settings.value("port").toUInt();

        addServer(name, host, port);
    }

    if (ui->listWidget->count() == 0)
        addServer("swgemu.com", defaultLoginAddress, defaultLoginPort);

    settings.endArray();
}

void LoginServers::addServer(const QString&name, const QString& host, quint16 port) {
    LoginServer* server = new LoginServer(name, host, port);

    if (ui->listWidget->count() % 2 == 0) {
        server->setBackgroundColor(QColor::fromRgb(230, 230, 230));
    }

    ui->listWidget->addItem(server);
}

void LoginServers::removeServer() {
    if (ui->listWidget->count() == 1) {
        QMessageBox::warning(this, "Error", "Need at least one login server!");
        return;
    }

    int row = ui->listWidget->currentRow();

    qDebug() << "current row " << row;

    LoginServer* server = dynamic_cast<LoginServer*>(ui->listWidget->item(row));

    if (server == NULL) {
        qDebug() << "server null";
        return;
    }

    if (server->getName() == "swgemu.com") {
        QMessageBox::warning(this, "Error", "Can't delete swgemu.com server!");
        return;
    }

    delete ui->listWidget->takeItem(row);
}

int LoginServers::count() {
    return ui->listWidget->count();
}

LoginServer* LoginServers::getServer(int index) {
    return dynamic_cast<LoginServer*>(ui->listWidget->item(index));
}

LoginServer* LoginServers::getServer(const QString& name) {
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        LoginServer* server = dynamic_cast<LoginServer*>(ui->listWidget->item(i));

        if (server == NULL)
            continue;

        if (server->text().toLower() == name.toLower())
            return server;
    }

    return NULL;
}

void LoginServers::editServer() {
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < 0)
        return;

    LoginServer* server = dynamic_cast<LoginServer*>(ui->listWidget->item(currentRow));

    if (server == NULL)
        return;

    AddLoginServer dialog(this);
    dialog.setHost(server->getHost());
    dialog.setName(server->getName());
    dialog.setPort(server->getPort());
    dialog.disableNameEdit();

    if (dialog.exec() == QDialog::Rejected)
        return;

    if (!checkServerDialog(dialog))
        return;

    server->setHost(dialog.getHost());
    server->setPort(dialog.getPort());
}

bool LoginServers::checkServerDialog(AddLoginServer& dialog) {
    if (dialog.getName().isEmpty()) {
        QMessageBox::warning(this, "Error", "Name value can't be empty!");
        return false;
    }

    if (dialog.getHost().isEmpty()) {
        QMessageBox::warning(this, "Error", "Host value can't be empty!");
        return false;
    }

    return true;
}

void LoginServers::addServer() {
    AddLoginServer dialog(this);

    if (dialog.exec() == QDialog::Rejected)
        return;

    bool res = checkServerDialog(dialog);

    if (!res)
        return;

    if (getServer(dialog.getName()) != NULL) {
        QMessageBox::warning(this, "Error", "A login server with that name already exists!");

        return;
    }

    //addServever()
    addServer(dialog.getName().trimmed(), dialog.getHost().trimmed(), dialog.getPort());
    //QString name = dialog.u
}

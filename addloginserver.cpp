#include "addloginserver.h"
#include "ui_addloginserver.h"

AddLoginServer::AddLoginServer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddLoginServer) {
  ui->setupUi(this);
}

AddLoginServer::~AddLoginServer() {
  delete ui;
}

QString AddLoginServer::getName() {
  return ui->lineEdit_name->text();
}

QString AddLoginServer::getHost() {
  return ui->lineEdit_host->text();
}

quint16 AddLoginServer::getPort() {
  return ui->spinBox_port->value();
}

void AddLoginServer::disableNameEdit() {
    ui->lineEdit_name->setEnabled(false);
}

void AddLoginServer::setName(const QString& name) {
    ui->lineEdit_name->setText(name);
}

void AddLoginServer::setHost(const QString& host) {
    ui->lineEdit_host->setText(host);
}

void AddLoginServer::setPort(quint16 port) {
    ui->spinBox_port->setValue(port);
}

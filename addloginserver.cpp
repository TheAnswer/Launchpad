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

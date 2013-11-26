#ifndef LOGINSERVERS_H
#define LOGINSERVERS_H

#include <QDialog>
#include "loginserver.h"
#include "addloginserver.h"

namespace Ui {
  class LoginServers;
}

class LoginServers : public QDialog
{
  Q_OBJECT
  
public:
  explicit LoginServers(QWidget *parent = 0);
  ~LoginServers();

  static QString defaultLoginAddress;
  static quint16 defaultLoginPort;

  void addServer(const QString& name, const QString& host, quint16 port);
  LoginServer* getServer(const QString& name);

  int count();
  LoginServer* getServer(int index);

  bool checkServerDialog(AddLoginServer& dialog);

public slots:
  void addServer();
  void removeServer();
  void reloadServers();
  void editServer();
  
private:
  Ui::LoginServers *ui;
};

#endif // LOGINSERVERS_H

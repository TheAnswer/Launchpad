#ifndef ADDLOGINSERVER_H
#define ADDLOGINSERVER_H

#include <QDialog>

namespace Ui {
  class AddLoginServer;
}

class AddLoginServer : public QDialog
{
  Q_OBJECT
  
public:
  explicit AddLoginServer(QWidget *parent = 0);
  ~AddLoginServer();

  QString getName();
  QString getHost();
  quint16 getPort();
  
private:
  Ui::AddLoginServer *ui;
};

#endif // ADDLOGINSERVER_H

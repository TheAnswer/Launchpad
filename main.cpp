#include "mainwindow.h"
#include <QApplication>
#include "singleinstance.h"
#include <QMessageBox>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

//  a.processEvents();

  //{{DCB15209-9E02-489D-9FD6-03689735BD49}}

//  QSharedMemory shared("DCB15209-9E02-489D-9FD6-03689735BD49");

//  if (!shared.create())

  //HANDLE instanceMutex = CreateMutex()

  SingleInstance instance(TEXT("DCB15209-9E02-489D-9FD6-03689735BD49"));

  if (instance.isAnotherInstanceRunning()) {
      QMessageBox::critical(NULL, "ERROR", "There is a launchpad instance running already!");
      return 1;
  }

  MainWindow w;
  w.show();

  return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#ifdef Q_OS_WIN32
#include "singleinstance.h"
#endif
#include <QMessageBox>
#include "utils.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

//  a.processEvents();

  //{{DCB15209-9E02-489D-9FD6-03689735BD49}}

//  QSharedMemory shared("DCB15209-9E02-489D-9FD6-03689735BD49");

//  if (!shared.create())

  //HANDLE instanceMutex = CreateMutex()



#ifdef Q_OS_WIN32
  SingleInstance instance(TEXT("DCB15209-9E02-489D-9FD6-03689735BD49"));

  if (instance.isAnotherInstanceRunning()) {
      QMessageBox::critical(NULL, "ERROR", "There is a launchpad instance running already, check your system tray!");
      return 1;
  }
#endif

  MainWindow w;
  w.show();

  return a.exec();
}

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
  class Settings;
}

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();

public slots:
  void selectFolder();

  void updateFolderSetting();
  void updateCloseAfterStartSetting();
  void updateMinimizeAfterStart();
  void updateCaptureDebugSetting();
  void updateMultiThreadedFullScan();
  void updateWineBinary();
  void updateWineArguments();
  void updateWineEnvironment();

  void restoreCloseAfterStart();
  void restoreMimizeAfterStart();
  void restoreCaptureDebug();
  void restoreMultiThreadedFullScan();
  void restoreFolder();
  void restoreWineBinary();
  void restoreWineArgs();
  void restoreWineEnvironment();

  void closeAfterStartChanged(bool state);
  void minimizeToTrayAfterStartChanged(bool state);


  void updateAllOptions();
  void restoreOptions();


  //void

private:
  Ui::Settings *ui;
};

#endif // SETTINGS_H

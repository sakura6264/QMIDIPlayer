#ifndef PREFERENCEWND_H
#define PREFERENCEWND_H

#include <QDesktopServices>
#include <QDialog>
#include <QList>
#include <QUrl>
#include <QString>

namespace Ui {
class PreferenceWnd;
}

class PreferenceWnd : public QDialog {
  Q_OBJECT

public:
  explicit PreferenceWnd(QWidget *parent = nullptr);
  void setUp(const QStringList &coreList, const QStringList &deviceList,
             QString currentCore, QString currentDevice, int mTime);
  QString getCore();
  QString getDevice();
  int getMTime();
  ~PreferenceWnd();

private slots:
  void on_buttonBox_rejected();

  void on_buttonBox_accepted();

  void on_EditRaw_clicked();

private:
  Ui::PreferenceWnd *ui;
  QStringList coreList;
  QStringList deviceList;
  QString currentCore;
  QString currentDevice;
  int mTime;
};

#endif // PREFERENCEWND_H

#ifndef PREFERENCES_H
#define PREFERENCES_H
#include "QMidi.h"
#include "irendercorefactory.h"
#include "preferencewnd.h"
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMessageBox>
#include <QString>
#include <QWidget>

class Preferences {
public:
  Preferences(QWidget *wdg);
  void ChangePreferences();
  QString getCoreName();
  QString getDevice();
  int getMTime();

private:
  void useDefault();
  void generateDefault();
  QString coreName;
  QString midiDevice;
  int mtime = 0;
  QWidget *parent;
};

#endif // PREFERENCES_H

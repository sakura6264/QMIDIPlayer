#include "preferences.h"
/* Class to manage the preferences.
 * Stored in json.
 */

Preferences::Preferences(QWidget *wdg) {
  this->parent = wdg;
  QFileInfo info(QCoreApplication::applicationDirPath() +
                 QString("/preferences.json"));
  if (!info.isFile()) {
    QMessageBox::critical(wdg, QString("Error"),
                          QString("Could Not Find preferences.json.The Program "
                                  "Will Run With Default Preferences."));
    this->generateDefault();
    this->useDefault();
    return;
  }
  QFile fin(QCoreApplication::applicationDirPath() +
            QString("/preferences.json"));
  if (!fin.open(QIODevice::Text | QIODevice::ReadOnly)) {
    QMessageBox::critical(wdg, QString("Error"),
                          QString("Could Not Open preferences.json.The Program "
                                  "Will Run With Default Preferences."));
    this->generateDefault();
    this->useDefault();
    return;
  }
  QJsonParseError perr;
  QJsonDocument jsonin = QJsonDocument::fromJson(fin.readAll(), &perr);
  if (perr.error != QJsonParseError::NoError) {
    QMessageBox::critical(
        wdg, QString("Error"),
        QString(
            "Json Parse Error.The Program Will Run With Default Preferences."));
    this->generateDefault();
    this->useDefault();
    fin.close();
    return;
  }
  QJsonObject obj = jsonin.object();
  QJsonValue v_corename = obj.value(QString("coreName"));
  QJsonValue v_mtime = obj.value(QString("mTime"));
  QJsonValue v_device = obj.value(QString("midiDevice"));
  if (!(v_corename.isString() && v_mtime.isDouble() && v_device.isString())) {
    QMessageBox::critical(
        wdg, QString("Error"),
        QString(
            "Json Data Error.The Program Will Run With Default Preferences."));
    this->generateDefault();
    this->useDefault();
    fin.close();
    return;
  }
  IRenderCoreFactory fctr;
  if (!(fctr.getMap().contains(v_corename.toString()) && v_mtime.toInt() > 0 &&
        QMidiOut::devices().keys(v_device.toString()).size() > 0)) {
    QMessageBox::critical(wdg, QString("Error"),
                          QString("Json Data Range Error.The Program Will Run "
                                  "With Default Preferences."));
    this->generateDefault();
    this->useDefault();
    fin.close();
    return;
  }
  this->coreName = v_corename.toString();
  this->mtime = int(v_mtime.toDouble() * 50);
  this->midiDevice = v_device.toString();
  fin.close();
}

void Preferences::ChangePreferences() {
  IRenderCoreFactory fctr;
  QStringList coreList;
  QMap<QString, RenderCoreID> coreMap = fctr.getMap();
  for (QMap<QString, RenderCoreID>::iterator it = coreMap.begin();
       it != coreMap.end(); it++) {
    coreList << it.key();
  }
  QMap<QString, QString> deviceMap = QMidiOut::devices();
  QStringList deviceList;
  for (QMap<QString, QString>::iterator it = deviceMap.begin();
       it != deviceMap.end(); it++) {
    deviceList << it.value();
  }
  PreferenceWnd *Wnd = new PreferenceWnd(parent);
  Wnd->setUp(coreList, deviceList, this->coreName, this->midiDevice,
             this->mtime);
  Wnd->exec();
  if (Wnd->getCore().isNull() || Wnd->getCore().isEmpty() ||
      Wnd->getDevice().isNull() || Wnd->getDevice().isEmpty() ||
      Wnd->getMTime() <= 0) {
    return;
  }
  this->coreName = Wnd->getCore();
  this->mtime = Wnd->getMTime();
  this->midiDevice = Wnd->getDevice();
  QFile fout(QCoreApplication::applicationDirPath() +
             QString("/preferences.json"));
  if (!fout.open(QIODevice::Text | QIODevice::WriteOnly)) {
    QMessageBox::critical(parent, QString("Error"),
                          QString("Could Not Create preferences.json"));
    this->generateDefault();
    this->useDefault();
    return;
  }
  QJsonDocument jsonout;
  QJsonObject obj;
  obj[QString("coreName")] = this->coreName;
  obj[QString("mTime")] = double(this->mtime) / 50;
  obj[QString("midiDevice")] = this->midiDevice;
  jsonout.setObject(obj);
  fout.write(jsonout.toJson(QJsonDocument::Indented));
  fout.close();
}

QString Preferences::getCoreName() { return this->coreName; }

QString Preferences::getDevice() { return this->midiDevice; }

int Preferences::getMTime() { return this->mtime; }

void Preferences::useDefault() {
  this->coreName = "Basic";
  this->mtime = 50;
  this->midiDevice = QMidiOut::devices().begin().value();
}

void Preferences::generateDefault() {
  QFile fout(QCoreApplication::applicationDirPath() +
             QString("/preferences.json"));
  if (!fout.open(QIODevice::Text | QIODevice::WriteOnly)) {
    QMessageBox::critical(parent, QString("Error"),
                          QString("Could Not Create preferences.json"));
    this->generateDefault();
    this->useDefault();
    return;
  }
  QJsonDocument jsonout;
  QJsonObject obj;
  obj[QString("coreName")] = QString("Basic");
  obj[QString("mTime")] = 1.0;
  obj[QString("midiDevice")] = QMidiOut::devices().begin().value();
  jsonout.setObject(obj);
  fout.write(jsonout.toJson(QJsonDocument::Indented));
  fout.close();
}

#include "preferencewnd.h"
#include "ui_preferencewnd.h"
/* The dialog of preferences.
 * managed by the class "Preferences".
 */

PreferenceWnd::PreferenceWnd(QWidget *parent)
    : QDialog(parent), ui(new Ui::PreferenceWnd) {
  ui->setupUi(this);
}

void PreferenceWnd::setUp(const QStringList &coreList,
                          const QStringList &deviceList, QString currentCore,
                          QString currentDevice, int mTime) {
  this->coreList = coreList;
  this->deviceList = deviceList;
  this->currentCore = currentCore;
  this->currentDevice = currentDevice;
  this->mTime = mTime;
  ui->coreCombo->addItems(coreList);
  ui->coreCombo->setCurrentIndex(coreList.indexOf(currentCore));
  ui->deviceCombo->addItems(deviceList);
  ui->deviceCombo->setCurrentIndex(deviceList.indexOf(currentDevice));
  ui->mTimeSpin->setValue(double(mTime) / 50);
}

QString PreferenceWnd::getCore() { return this->currentCore; }

QString PreferenceWnd::getDevice() { return this->currentDevice; }

int PreferenceWnd::getMTime() { return this->mTime; }

PreferenceWnd::~PreferenceWnd() { delete ui; }

void PreferenceWnd::on_buttonBox_rejected() {
  this->currentCore = QString();
  this->mTime = 0;
}

void PreferenceWnd::on_buttonBox_accepted() {
  this->currentCore = this->coreList[ui->coreCombo->currentIndex()];
  this->currentDevice = this->deviceList[ui->deviceCombo->currentIndex()];
  this->mTime = int(ui->mTimeSpin->value() * 50);
}

void PreferenceWnd::on_EditRaw_clicked() {
  QDesktopServices::openUrl(QUrl(QString("file:///") +
                                 QCoreApplication::applicationDirPath() +
                                 QString("/preferences.json")));
}

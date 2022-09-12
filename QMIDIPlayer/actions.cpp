#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 * Collection of some of the Slots function's implements.
 * Put them all into the "mainwindow.cpp" doesn't sound like a good idea.
 */

void MainWindow::OpenAct() {
  QString s = QFileDialog::getOpenFileName(this, QString("Select Midi File"),
                                           ".", "*.mid");
  if (s.isNull() || s.isEmpty()) {
    return;
  }
  QFileInfo sinfo(s);
  if (!sinfo.isFile()) {
    QMessageBox::critical(this, QString("Error"),
                          QString("File doen't exist."));
    return;
  }
  this->changeFile(s);
}
void MainWindow::ExitAct() { QApplication::exit(); }
void MainWindow::GotoAct() {
  if (this->m_SDLRender->getFileEmpty()) {
    QMessageBox::warning(this, QString("Warning"),
                         QString("No File is Opened."));
    return;
  }
  int max = this->m_SDLRender->getFileTime();
  if (max <= 0) {
    QMessageBox::warning(this, QString("Warning"),
                         QString("No File is Opened."));
    return;
  }
  this->gotownd->init(max);
  this->gotownd->exec();
  int value = gotownd->getresult();
  if (value > 0 && value < max) {
    emit this->sendToRender(SignalType::Set, value);
  }
}
void MainWindow::AnalyzeAct() {
  if (this->m_SDLRender->getFileEmpty()) {
    QMessageBox::warning(this, QString("Warning"),
                         QString("No File is Opened."));
    return;
  }
  this->analyzer->setString(this->m_SDLRender->getAnalyze());
  this->analyzer->exec();
}
void MainWindow::PreferenceAct() { this->pref->ChangePreferences(); }
void MainWindow::DocumentAct() {
  QDesktopServices::openUrl(QUrl(QString("file:///") +
                                 QCoreApplication::applicationDirPath() +
                                 QString("/README.txt")));
}
void MainWindow::AboutAct() { QMessageBox::aboutQt(this); }

void MainWindow::ChangeSlider(int pos) {
  this->ui->sliderTime->setValue(pos);
  QString labelText =
      QString("%1:%2/%3:%4")
          .arg(ui->sliderTime->value() / 3000)
          .arg((ui->sliderTime->value() % 3000) / 50, 2, 10, QLatin1Char('0'))
          .arg(ui->sliderTime->maximum() / 3000)
          .arg((ui->sliderTime->maximum() % 3000) / 50, 2, 10,
               QLatin1Char('0'));
  ui->labelTime->setText(labelText);
}

void MainWindow::EditSlider(int length) {
  this->ui->sliderTime->setMaximum(length);
  QString labelText =
      QString("%1:%2/%3:%4")
          .arg(ui->sliderTime->value() / 3000)
          .arg((ui->sliderTime->value() % 3000) / 50, 2, 10, QLatin1Char('0'))
          .arg(ui->sliderTime->maximum() / 3000)
          .arg((ui->sliderTime->maximum() % 3000) / 50, 2, 10,
               QLatin1Char('0'));
  ui->labelTime->setText(labelText);
}

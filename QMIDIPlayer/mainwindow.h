#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "analyzer.h"
#include "gotownd.h"
#include "preferences.h"
#include "sdlrender.h"
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMetaType>
#include <QShortcut>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QTimer *renderTimer;
  SDLRender *m_SDLRender;
  GotoWnd *gotownd;
  Analyzer *analyzer;
  Preferences *pref;
  QShortcut *scLeft;
  QShortcut *scRight;
  QShortcut *scSpace;

private slots:
  void OpenAct();
  void ExitAct();
  void GotoAct();
  void AnalyzeAct();
  void PreferenceAct();
  void DocumentAct();
  void AboutAct();
  void on_pushButtonOnPause_clicked();

  void on_pushButtonVol_clicked();

  void on_pushButtonFoward_clicked();

  void on_pushButtonStop_clicked();

  void on_pushButtonBack_clicked();

  void on_pushButtonPrev_clicked();

  void on_sliderTime_sliderReleased();

  void on_sliderTime_sliderPressed();

public slots:
  void ChangeSlider(int pos);
  void EditSlider(int length);
signals:
  void sendToRender(SignalType signal, int val);
  void changeFile(QString file);
};
#endif // MAINWINDOW_H

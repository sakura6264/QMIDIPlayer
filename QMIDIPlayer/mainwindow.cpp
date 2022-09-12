#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 * Main window.Mainly for connecting the signals.
 * The slots for the buttons are also here.
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  this->pref = new Preferences(this);
  qRegisterMetaType<QPair<QList<QMidiEvent *>, int>>(
      "QPair<QList<QMidiEvent*>,int>&");
  qRegisterMetaType<QList<RenderEvent>>("QList<RenderEvent>&");
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenAct()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(ExitAct()));
  connect(ui->actionGoto, SIGNAL(triggered()), this, SLOT(GotoAct()));
  connect(ui->actionAnalyze, SIGNAL(triggered()), this, SLOT(AnalyzeAct()));
  connect(ui->actionPreference, SIGNAL(triggered()), this,
          SLOT(PreferenceAct()));
  connect(ui->actionDocument, SIGNAL(triggered()), this, SLOT(DocumentAct()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(AboutAct()));
  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
  setFixedSize(this->width(), this->height());
  m_SDLRender = new SDLRender(this);
  m_SDLRender->initSDLRender(this->ui->SDLWidget->winId(), pref->getCoreName(),
                             pref->getDevice());
  renderTimer = new QTimer(this);
  renderTimer->setTimerType(Qt::PreciseTimer);
  connect(renderTimer, SIGNAL(timeout()), m_SDLRender, SLOT(renderLoop()));
  connect(this, SIGNAL(sendToRender(SignalType, int)), m_SDLRender,
          SLOT(signalReceiver(SignalType, int)));
  connect(this, SIGNAL(changeFile(QString)), m_SDLRender,
          SLOT(changeFile(QString)));
  connect(m_SDLRender, SIGNAL(callButton()), ui->pushButtonOnPause,
          SIGNAL(clicked()));
  connect(m_SDLRender, SIGNAL(callSlider(int)), this, SLOT(ChangeSlider(int)));
  connect(m_SDLRender, SIGNAL(setSlider(int)), this, SLOT(EditSlider(int)));
  this->gotownd = new GotoWnd(this);
  this->analyzer = new Analyzer(this);
  this->scLeft = new QShortcut(QKeySequence(Qt::Key_Left), this);
  this->scRight = new QShortcut(QKeySequence(Qt::Key_Right), this);
  this->scSpace = new QShortcut(QKeySequence(Qt::Key_Space), this);
  scSpace->setAutoRepeat(false);
  connect(this->scLeft, SIGNAL(activated()), ui->pushButtonBack,
          SIGNAL(clicked()));
  connect(this->scRight, SIGNAL(activated()), ui->pushButtonFoward,
          SIGNAL(clicked()));
  connect(this->scSpace, SIGNAL(activated()), ui->pushButtonOnPause,
          SIGNAL(clicked()));
  renderTimer->start(20);
}

MainWindow::~MainWindow() {
  renderTimer->stop();
  delete renderTimer;
  delete scLeft;
  delete scRight;
  delete scSpace;
  delete m_SDLRender;
  delete gotownd;
  delete analyzer;
  delete pref;
  delete ui;
}

void MainWindow::on_pushButtonOnPause_clicked() {
  if (this->m_SDLRender->getFileEmpty()) {
    return;
  }
  QIcon ico;
  if (this->m_SDLRender->getStatus()) {
    ico.addFile(QString(":/res/play.svg"));
  } else {
    ico.addFile(QString(":/res/pause.svg"));
  }
  ui->pushButtonOnPause->setIcon(ico);
  emit sendToRender(SignalType::OnStop, 0);
}

void MainWindow::on_pushButtonVol_clicked() {
  QIcon ico;
  if (this->m_SDLRender->getVol()) {
    ico.addFile(QString(":/res/volume-off.svg"));
  } else {
    ico.addFile(QString(":/res/volume-loud.svg"));
  }
  ui->pushButtonVol->setIcon(ico);
  emit sendToRender(SignalType::Vol, 0);
}

void MainWindow::on_pushButtonFoward_clicked() {
  emit sendToRender(SignalType::SetAdd, pref->getMTime());
}

void MainWindow::on_pushButtonStop_clicked() { emit changeFile(QString()); }

void MainWindow::on_pushButtonBack_clicked() {
  emit sendToRender(SignalType::SetAdd, -pref->getMTime());
}

void MainWindow::on_pushButtonPrev_clicked() {
  emit sendToRender(SignalType::Set, 0);
}

void MainWindow::on_sliderTime_sliderReleased() {
  connect(m_SDLRender, SIGNAL(callSlider(int)), this, SLOT(ChangeSlider(int)));
  emit sendToRender(SignalType::Set, ui->sliderTime->value());
  QString labelText =
      QString("%1:%2/%3:%4")
          .arg(ui->sliderTime->value() / 3000)
          .arg((ui->sliderTime->value() % 3000) / 50, 2, 10, QLatin1Char('0'))
          .arg(ui->sliderTime->maximum() / 3000)
          .arg((ui->sliderTime->maximum() % 3000) / 50, 2, 10,
               QLatin1Char('0'));
  ui->labelTime->setText(labelText);
}

void MainWindow::on_sliderTime_sliderPressed() {
  disconnect(m_SDLRender, SIGNAL(callSlider(int)), this,
             SLOT(ChangeSlider(int)));
}

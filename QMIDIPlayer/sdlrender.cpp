#include "sdlrender.h"
/* Manage how to control the renderer.
 * The true Core of this program.
 */
SDLRender::SDLRender(QObject *parent) : QObject(parent) {
  this->timer = 0;
  this->playing = false;
  this->inited = false;
  this->tasksindex = 1;
}

SDLRender::~SDLRender() {
  if (inited) {
    if (this->ren != nullptr) {
      SDL_DestroyRenderer(this->ren);
    }
    if (this->screen != nullptr) {
      SDL_DestroyWindow(this->screen);
    }
    if (this->renderCore != nullptr) {
      // this->renderCore->destroy();
      delete this->renderCore;
      this->renderCore = nullptr;
    }
    if (this->midi_file != nullptr) {
      delete this->midi_file;
      this->midi_file = nullptr;
    }
    SDL_Quit();
  }
}

void SDLRender::initSDLRender(WId winId, QString core, QString midiDevice) {
  if (inited) {
    return;
  }
  SDL_Init(SDL_INIT_EVERYTHING);
  this->screen = SDL_CreateWindowFrom((void *)winId);
  this->ren = SDL_CreateRenderer(
      screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  this->midiout = new QMidiOut;
  this->midiout->connect(*QMidiOut::devices().keys(midiDevice).begin());
  IRenderCoreFactory factory;
  this->renderCore = factory.getRenderCore(core);
  this->renderCore->init(this->ren, this->screen, this->midiout);
  this->midi_file = new QMidiFile;
  this->inited = true;
}

bool SDLRender::getStatus() { return this->playing; }

bool SDLRender::getVol() { return this->vol; }

bool SDLRender::getFileEmpty() {
  return (this->currentFile.isEmpty() || this->currentFile.isNull());
}

int SDLRender::getFileTime() {
  if (this->midi_file == nullptr) {
    return 0;
  }
  int ret = int(
      midi_file->timeFromTick((*(this->midi_file->events()).rbegin())->tick()) *
      50);
  return (ret > 0 ? ret : 0);
}

QString SDLRender::getAnalyze() {
  QString Div;
  switch (midi_file->divisionType()) {
  case QMidiFile::DivisionType::Invalid:
    Div = QString("Invalid");
    break;
  case QMidiFile::DivisionType::PPQ:
    Div = QString("PPQ");
    break;
  case QMidiFile::DivisionType::SMPTE24:
    Div = QString("SMPTE24");
    break;
  case QMidiFile::DivisionType::SMPTE25:
    Div = QString("SMPTE25");
    break;
  case QMidiFile::DivisionType::SMPTE30DROP:
    Div = QString("SMPTE30DROP");
    break;
  case QMidiFile::DivisionType::SMPTE30:
    Div = QString("SMPTE30");
    break;
  }
  QString msg = QString("File Name :\n%1\nLength : %2 min %3 s %4 ms\nEvents : "
                        "%5\nTracks : %6\nDivision Type : %7");
  msg = msg.arg(*currentFile.split(QString("/")).rbegin())
            .arg(getFileTime() / 3000)
            .arg((getFileTime() / 50) % 60)
            .arg(int(midi_file->timeFromTick(
                         (*midi_file->events().rbegin())->tick()) *
                     1000) %
                 1000)
            .arg(midi_file->events().size())
            .arg(midi_file->tracks().size())
            .arg(Div);
  return msg;
}

void SDLRender::renderLoop() {

  if (!playing) {
    SDL_RenderPresent(ren);
    return;
  }
  emit callSlider(timer);
  if (miditasks.isEmpty() && rendertasks.isEmpty()) {
    SDL_RenderPresent(ren);
    timer = 0;
    tasksindex = 0;
    return;
  }
  QList<QMidiEvent *> mtasks;
  if (timer > miditasks.rbegin()->second) {
    if (this->playing) {
      emit this->callButton();
      emit callSlider(0);
      this->timer = 0;
      tasksindex = 0;
    }
    return;
  }
  if (tasksindex >= miditasks.size()) {
    return;
  }
  while (timer > miditasks[tasksindex].second) {
    tasksindex++;
  }
  if (timer == miditasks[tasksindex].second) {
    mtasks = miditasks[tasksindex].first;
  }
  this->renderCore->render(this->timer, this->vol, &mtasks, this->rendertasks);
  timer++;
}

void SDLRender::signalReceiver(SignalType signal, int val) {
  switch (signal) {
  case OnStop:
    this->playing = !playing;
    this->midiout->stopAll();
    break;
  case Vol:
    this->vol = !vol;
    this->midiout->stopAll();
    break;
  case Set:
    if (!(this->currentFile.isEmpty() || this->currentFile.isNull())) {
      int timerto = val;
      timerto = timerto > 0 ? timerto : 0;
      timerto =
          timerto < rendertasks.size() ? timerto : (rendertasks.size() - 1);
      this->timer = timerto;
      tasksindex = 0;
      emit callSlider(timerto);
    }
    break;
  case SetAdd:
    if (!(this->currentFile.isEmpty() || this->currentFile.isNull())) {
      int timerto = this->timer + val;
      timerto = timerto > 0 ? timerto : 0;
      timerto =
          timerto < rendertasks.size() ? timerto : (rendertasks.size() - 1);
      this->timer = timerto;
      tasksindex = 0;
      emit callSlider(timerto);
    }
    break;
  }
}

void SDLRender::changeFile(QString file) {
  this->timer = 0;
  emit callSlider(0);
  emit setSlider(0);
  if (playing) {
    emit this->callButton();
  }
  midiout->stopAll();
  SDL_RenderClear(ren);
  this->currentFile = file;
  this->tasksindex = 0;
  if (this->midi_file != nullptr) {
    delete this->midi_file;
    this->midi_file = nullptr;
  }
  this->miditasks.clear();
  this->rendertasks.clear();
  if (this->currentFile.isEmpty() || this->currentFile.isNull()) {
    return;
  }
  this->midi_file = new QMidiFile;
  bool successfulLoad = midi_file->load(currentFile);
  MIDIDecoder *midi = new MIDIDecoder(this);
  connect(midi, SIGNAL(addMidiTask(QPair<QList<QMidiEvent *>, int> &)), this,
          SLOT(addMidiTask(QPair<QList<QMidiEvent *>, int> &)));
  connect(midi, SIGNAL(addRenderTask(QList<RenderEvent> &)), this,
          SLOT(addRenderTask(QList<RenderEvent> &)));
  if (!successfulLoad) {
    QMessageBox::critical(NULL, QString("Error"), QString("Illegal Midi File"));
    this->currentFile.clear();
    return;
  }
  int timeofmidi =
      midi_file->timeFromTick((*(midi_file->events().rbegin()))->tick()) * 50;
  emit setSlider(timeofmidi);
  this->miditasks.reserve(this->midi_file->events().size());
  this->rendertasks.reserve(timeofmidi);
  QProgressDialog *pDialog = new QProgressDialog;
  pDialog->setWindowFlag(Qt::Dialog);
  pDialog->setWindowTitle("Load MIDI");
  pDialog->setLabelText("Loading...");
  pDialog->setAutoClose(true);
  connect(pDialog, SIGNAL(canceled()), midi, SLOT(terminate()));
  connect(midi, SIGNAL(setProgress(int)), pDialog, SLOT(setMaximum(int)));
  connect(midi, SIGNAL(changeProgress(int)), pDialog, SLOT(setValue(int)));
  midi->init(this->midi_file);
  midi->start();
  pDialog->exec();
  if (!this->playing) {
    emit callButton();
  }
  midi->disconnect();
  delete midi;
  pDialog->disconnect();
  delete pDialog;
}

void SDLRender::addMidiTask(QPair<QList<QMidiEvent *>, int> &data) {
  this->miditasks.push_back(data);
}

void SDLRender::addRenderTask(QList<RenderEvent> &data) {
  this->rendertasks.push_back(data);
}

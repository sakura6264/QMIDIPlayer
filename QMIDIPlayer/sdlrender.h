#ifndef SDLRENDER_H
#define SDLRENDER_H
#include "irendercorefactory.h"
#include "mididecoder.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QProgressDialog>
#include <SDL2/SDL.h>

enum SignalType { OnStop, Set, SetAdd, Vol };

class SDLRender : public QObject {
  Q_OBJECT
public:
  explicit SDLRender(QObject *parent = nullptr);
  ~SDLRender();
  void initSDLRender(WId winId, QString core, QString midiDevice);
  bool getStatus();
  bool getVol();
  bool getFileEmpty();
  int getFileTime();
  QString getAnalyze();
signals:
  void callSlider(int pos);
  void setSlider(int length);
  void callButton();
public slots:
  void renderLoop();
  void signalReceiver(SignalType signal, int val);
  void changeFile(QString file);
  void addMidiTask(QPair<QList<QMidiEvent *>, int> &data);
  void addRenderTask(QList<RenderEvent> &data);

private:
  QMidiOut *midiout = nullptr;
  SDL_Window *screen = nullptr;
  SDL_Renderer *ren = nullptr;
  QString currentFile;
  QMidiFile *midi_file = nullptr;
  QList<QPair<QList<QMidiEvent *>, int>> miditasks;
  int tasksindex = 1;
  QList<QList<RenderEvent>> rendertasks;
  IRenderCore *renderCore = nullptr;
  bool playing = false;
  bool inited = false;
  int timer = 0;
  bool vol = true;
};

#endif // SDLRENDER_H

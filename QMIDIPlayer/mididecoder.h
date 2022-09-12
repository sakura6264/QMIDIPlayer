#ifndef MIDIDECODER_H
#define MIDIDECODER_H
#include "QMidi.h"
#include "irendercore.h"
#include <QList>
#include <QReadWriteLock>
#include <QThread>

class MIDIDecoder : public QThread {
  Q_OBJECT
public:
  explicit MIDIDecoder(QObject *parent = nullptr);
  void init(QMidiFile *file);
  void run() override;
  void destroy();
signals:
  void addMidiTask(QPair<QList<QMidiEvent *>, int> &data);
  void addRenderTask(QList<RenderEvent> &data);
  void setProgress(int max);
  void changeProgress(int process);

private:
  QMidiFile *file;
  bool inited = false;
};

#endif // MIDIDECODER_H

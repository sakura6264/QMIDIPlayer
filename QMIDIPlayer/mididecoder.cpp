#include "mididecoder.h"
/*
 * Decode the midi events in a new thread.
 * Generate the render tasks and the midi tasks.
 */

MIDIDecoder::MIDIDecoder(QObject *parent) : QThread(parent) {}

void MIDIDecoder::init(QMidiFile *file) {
  if (this->inited) {
    return;
  }
  this->file = file;
}

void MIDIDecoder::run() {
  QList<QMidiEvent *> midievents = file->events();
  int renderTasksNum =
      file->timeFromTick((*(midievents.rbegin()))->tick()) * 50;
  int midiTasksNum = midievents.size();
  int allTasksNum = renderTasksNum + midiTasksNum;
  emit this->setProgress(allTasksNum);
  int taskNum = 0;
  int indexer = 0;
  QList<RenderEvent> renderevents;
  for (int timer = 0; timer < renderTasksNum; timer++) {
    for (QList<RenderEvent>::iterator itr = renderevents.begin();
         itr != renderevents.end(); itr++) {
      itr->vol = (itr->vol) * 0.95;
    }
    while (int(file->timeFromTick(midievents[indexer]->tick()) * 50) < timer) {
      indexer++;
    }
    while (int(file->timeFromTick(midievents[indexer]->tick()) * 50) == timer) {
      QMidiEvent *midevent = midievents[indexer];
      bool ishave = false;
      switch (midevent->type()) {
      case QMidiEvent::EventType::NoteOn:
        for (int itf = 0; itf < renderevents.size(); itf++) {
          if (renderevents[itf].place == midevent->note()) {
            renderevents[itf].vol = midevent->velocity();
            renderevents[itf].voice = midevent->voice();
            ishave = true;
            itf = renderevents.size();
          }
        }
        if (!ishave) {
          RenderEvent evt = {midevent->note(), midevent->voice(),
                             midevent->velocity()};
          renderevents.push_back(evt);
        }
        break;
      case QMidiEvent::EventType::NoteOff:
        for (int itf = 0; itf < renderevents.size(); itf++) {
          if (renderevents[itf].place == midevent->note()) {
            renderevents.removeAt(itf);
            itf = renderevents.size();
          }
        }
        break;
      default:
        break;
      }
      indexer++;
    }
    taskNum++;
    emit this->addRenderTask(renderevents);
    emit this->changeProgress(taskNum);
  }
  for (indexer = 0; indexer < midiTasksNum; indexer++) {
    QPair<QList<QMidiEvent *>, int> mid;
    mid.first.push_back(midievents[indexer]);
    mid.second = int(file->timeFromTick(midievents[indexer]->tick()) * 50);
    while ((indexer < midiTasksNum - 1) &&
           (int(file->timeFromTick(midievents[indexer + 1]->tick()) * 50) ==
            mid.second)) {
      indexer++;
      mid.first.push_back(midievents[indexer]);
    }
    taskNum++;
    emit this->addMidiTask(mid);
    emit this->changeProgress(taskNum);
  }
  emit this->changeProgress(allTasksNum);
}

void MIDIDecoder::destroy() {
  if (!this->inited) {
    return;
  }
}

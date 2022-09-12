#ifndef IRENDERCORE_H
#define IRENDERCORE_H
#include "QMidi.h"
#include <QList>
#include <SDL2/SDL.h>
/*The interface of the renderer.
 *Function render() will be use to render graphicS on the Qt Widget.
 *Must use the function init() to initialize the class;
 */
struct RenderEvent {
  int place;
  int voice;
  int vol;
};
class IRenderCore {
public:
  virtual void init(SDL_Renderer *ren, SDL_Window *screen,
                    QMidiOut *midiout) = 0;
  // virtual void destroy()=0;
  virtual ~IRenderCore() {}
  virtual void render(int time, bool vol, const QList<QMidiEvent *> *miditasks,
                      const QList<QList<RenderEvent>> &rendertask) = 0;
};

#endif // IRENDERCORE_H

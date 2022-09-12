#ifndef RENDERCOREBLUE_H
#define RENDERCOREBLUE_H
#include "irendercore.h"
namespace RenderCores {
class RenderCoreBlue : public IRenderCore {
public:
  void init(SDL_Renderer *ren, SDL_Window *screen, QMidiOut *midiout) override;
  void render(int time, bool vol, const QList<QMidiEvent *> *tasks,
              const QList<QList<RenderEvent>> &rendertask) override;
  ~RenderCoreBlue() override;

private:
  QMidiOut *midiout;
  SDL_Window *screen = nullptr;
  SDL_Renderer *ren = nullptr;
  SDL_Texture *line = nullptr;
  SDL_Rect linerect;
  int screen_w = 0;
  int screen_h = 0;
  bool inited = false;
};
} // namespace RenderCores

#endif // RENDERCOREBLUE_H

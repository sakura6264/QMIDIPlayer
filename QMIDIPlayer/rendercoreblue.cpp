#include "rendercoreblue.h"
/* Another implement of "IRenderCore".
 * Almost the same as RenderCoreBasic, only to test if the program can work
 * properly. Color is blue.
 */
#define notew 7
#define noteh 3
void RenderCores::RenderCoreBlue::init(SDL_Renderer *ren, SDL_Window *screen,
                                       QMidiOut *midiout) {
  this->screen = screen;
  this->ren = ren;
  this->midiout = midiout;
  SDL_GetWindowSize(screen, &screen_w, &screen_h);
  SDL_Surface *linesuf = SDL_CreateRGBSurfaceWithFormat(0, screen_w, 20, 32,
                                                        SDL_PIXELFORMAT_RGBA32);
  SDL_LockSurface(linesuf);
  for (int i = 0; i < linesuf->h; i++) {
    int colorn = (linesuf->h - i) * 12;
    Uint32 color = SDL_MapRGBA(linesuf->format, colorn, colorn, colorn, 200);
    for (int j = 0; j < linesuf->pitch / 4; j++) {
      ((Uint32 *)(linesuf->pixels))[i * (linesuf->pitch) / 4 + j] = color;
    }
  }
  SDL_UnlockSurface(linesuf);
  this->line = SDL_CreateTextureFromSurface(ren, linesuf);
  this->linerect.w = linesuf->w;
  this->linerect.h = linesuf->h;
  this->linerect.x = 0;
  this->linerect.y = screen_h * 10 / 11 + 1;
  SDL_FreeSurface(linesuf);
  this->inited = true;
}

void RenderCores::RenderCoreBlue::render(
    int time, bool vol, const QList<QMidiEvent *> *miditasks,
    const QList<QList<RenderEvent>> &rendertask) {
  SDL_RenderClear(ren);
  SDL_Surface *surface =
      SDL_CreateRGBSurface(0, screen_w, linerect.y - 1, 32, 0, 0, 0, 0);
  SDL_LockSurface(surface);
  for (int pos = 0;
       pos <
       ((rendertask.size() - time) < 200 ? (rendertask.size() - time) : 200);
       pos++) {
    for (RenderEvent evt : rendertask[time + pos]) {
      Uint32 color =
          SDL_MapRGBA(surface->format, evt.voice * 2, 0, evt.vol * 2, 255);
      for (int y = linerect.y - 1 - pos * noteh;
           y > linerect.y - 1 - pos * noteh - noteh; y--) {
        for (int x =
                 (screen_w - (notew + 1) * 128) / 2 + evt.place * (notew + 1);
             x < (screen_w - (notew + 1) * 128) / 2 + evt.place * (notew + 1) +
                     notew;
             x++) {
          ((Uint32 *)surface->pixels)[y * surface->pitch / 4 + x] = color;
        }
      }
    }
  }
  SDL_UnlockSurface(surface);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
  SDL_Rect rect = {0, 0, screen_w, linerect.y - 1};
  if (vol) {
    for (QMidiEvent *evt : *miditasks) {
      this->midiout->sendEvent(*evt);
    }
  }
  SDL_RenderCopy(ren, tex, NULL, &rect);
  SDL_RenderCopy(ren, line, NULL, &linerect);
  SDL_RenderPresent(ren);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(tex);
}

RenderCores::RenderCoreBlue::~RenderCoreBlue() {
  if (inited) {
    if (this->line != nullptr) {
      SDL_DestroyTexture(this->line);
    }
  }
}

#ifndef DATA_DEFINED
#define DATA_DEFINED

#include <SDL2/SDL.h>

struct Position {
  float x;
  float y;
};

struct Size {
  int w;
  int h;
};

struct Texture {
  SDL_Texture* texture;
  SDL_Rect rect;
};

#endif

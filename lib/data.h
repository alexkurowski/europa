#ifndef DATA_DEFINED
#define DATA_DEFINED

#include <SDL2/SDL.h>

#define TERMINAL_COUNT 4

#define TERMINAL_SCREEN_WIDTH 320
#define TERMINAL_SCREEN_HEIGHT 200


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

#pragma once

#ifndef DATA_DEFINED
#define DATA_DEFINED

#include <SDL2/SDL.h>
#include <array>

#define TERMINAL_COUNT 4

#define TERMINAL_SCREEN_WIDTH 320
#define TERMINAL_SCREEN_HEIGHT 200

typedef std::array<std::array<uint8_t, 3>, 16> colorMap;
typedef std::array<uint8_t, 2>                 colorBit;


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
  SDL_Rect     rect;
};


uint8_t getBitAt(uint8_t, uint8_t);

uint8_t getLeftByte(uint8_t);

uint8_t getRightByte(uint8_t val);

#endif

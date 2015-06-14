#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include "../../../lib/data.h"
#include "../../../lib/graphics.h"


#define TERMINAL_SCREEN_WIDTH  320
#define TERMINAL_SCREEN_HEIGHT 200

#define SHELL_SCREEN_SIZE      0x03E8
#define SHELL_CHAR_SIZE        0x0008
#define SHELL_FONT_SIZE        0x0400
#define BITMAP_SCREEN_SIZE     0x7D00

#define BACKGROUND_MARGIN      40


class Display {
  public:
    Display();
    ~Display();

    void draw(uint8_t,
              colorMap,
              colorBit,
              uint8_t*,
              uint8_t*,
              uint8_t);

  private:
    void drawBackground();
    void drawShell(uint8_t*, uint8_t*);
    void drawBitmap(uint8_t*);

    Position offset;

    uint8_t alpha;
    colorMap colors;
    colorBit currentColors;
};

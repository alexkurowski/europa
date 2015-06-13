#include "terminal.h"

Terminal::Terminal() {
  offset = Graphics::I()->getScreenScaledOffset();
}

Terminal::~Terminal() {}

void Terminal::update() {
  int min = 0;
  int max = 255;

  for (int i = 0; i < TERMINAL_MEMORY; i++) {
    memory[i] = randomRange(min, max);
  }
}

void Terminal::draw(int alpha) {
  drawBackground(alpha);
  drawScreen(alpha);
}

//=============================================================================

void Terminal::drawBackground(int alpha) {
  Graphics::I()->setColor(0, 0, 0, alpha);
  Graphics::I()->rectangle(offset.x - margin,
                           offset.y - margin,
                           TERMINAL_WIDTH + margin * 2,
                           TERMINAL_HEIGHT + margin * 2);
}

void Terminal::drawScreen(int alpha) {
  p = 0x2000;
  drawBitmap(alpha);
}

void Terminal::drawBitmap(alpha) {
  uint8_t bit;
  bool left = true;

  for (int i = 0; i < TERMINAL_WIDTH; i++) {
    for (int j = 0; j < TERMINAL_HEIGHT; j++) {
      bit = memory[p];
      if (left) {
        bit = bit >> 4;
      } else {
        bit = bit << 4;
        bit = bit >> 4;
        p++;
      }
      left = !left;

      Graphics::I()->setColor(colors[bit][0], colors[bit][1], colors[bit][2], alpha);
      Graphics::I()->point(i + offset.x, j + offset.y);
    }
  }
}

//=============================================================================

int Terminal::randomRange(int low, int high) {
  return static_cast<int>(random() * (high - low)) + low;
}


#include "display.h"

Display::Display(Memory* m) {
  mem = m;
  offset = Graphics::I()->getScreenScaledOffset();
}

Display::~Display() {}

void Display::draw(uint8_t mAlpha) {
  colors        = mem->getColors();
  currentColors = mem->getCurrentColors();
  alpha         = mAlpha;

  drawBackground();
  switch (mem->screenMode()) {
    case 0: // shell
      drawShell(mem->screenAddress(),
                mem->fontAddress(),
                mem->isAcceptInput(),
                mem->inputPosition());
      break;
    case 1: // bitmap
      drawBitmap(mem->screenAddress());
      break;
  }
}

void Display::drawBackground() {
  Graphics::I()->setColor(colors[currentColors[0]][0],
                          colors[currentColors[0]][1],
                          colors[currentColors[0]][2], alpha);
  Graphics::I()->rectangle(offset.x - BACKGROUND_MARGIN,
                           offset.y - BACKGROUND_MARGIN,
                           TERMINAL_SCREEN_WIDTH + BACKGROUND_MARGIN * 2,
                           TERMINAL_SCREEN_HEIGHT + BACKGROUND_MARGIN * 2);
}

void Display::drawShell(uint8_t* screen,
                        uint8_t* font,
                        bool acceptInput,
                        Position inputPosition) {
  uint16_t p = 0;
  uint16_t _p;
  uint16_t x, y;
  uint8_t ch, bit, col;
  x = 0;
  y = 0;
  while (p < SHELL_SCREEN_SIZE) {
    ch = screen[p];
    _p = ch * SHELL_CHAR_SIZE;

    for (int j = 0; j < SHELL_CHAR_SIZE; j++) {
      for (int i = 0; i < SHELL_CHAR_SIZE; i++) {
        bit = getBitAt(font[_p + j], 7 - i);

        col = currentColors[bit ^ 1];

        if (acceptInput)
          if (p == inputPosition.y * TERMINAL_SHELL_WIDTH + inputPosition.x)
            col = currentColors[bit];

        Graphics::I()->setColor(colors[col][0],
                                colors[col][1],
                                colors[col][2], alpha);
        Graphics::I()->point(x + i + offset.x,
                             y + j + offset.y);
      }
    }

    x += SHELL_CHAR_SIZE;
    if (x >= TERMINAL_SCREEN_WIDTH) {
      x = 0;
      y += SHELL_CHAR_SIZE;
    }
    p++;
  }
}

void Display::drawBitmap(uint8_t* screen) {
  uint16_t p = 0;
  uint8_t byte;
  bool left = true;

  for (int j = 0; j < TERMINAL_SCREEN_HEIGHT; j++) {
    for (int i = 0; i < TERMINAL_SCREEN_WIDTH; i++) {
      if (left) { // 0xF0 & byte
        byte = getLeftByte(screen[p]);
      } else {    // 0x0F & byte
        byte = getRightByte(screen[p]);
        p++;
      }
      left = !left;

      Graphics::I()->setColor(colors[byte][0], colors[byte][1], colors[byte][2], alpha);
      Graphics::I()->point(i + offset.x, j + offset.y);
    }
  }
}

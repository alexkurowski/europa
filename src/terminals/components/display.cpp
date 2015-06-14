#include "display.h"

Display::Display() {
  offset = Graphics::I()->getScreenScaledOffset();
}

Display::~Display() {}

void Display::draw(uint8_t  mode,
                   colorMap memoryColors,
                   colorBit memoryCurrentColors,
                   uint8_t* font,
                   uint8_t* screen,
                   bool     acceptInput,
                   Position inputPosition,
                   uint8_t  memoryAlpha) {

  colors        = memoryColors;
  currentColors = memoryCurrentColors;
  alpha         = memoryAlpha;

  drawBackground();
  switch (mode) {
    case 0: // shell
      drawShell(screen, font, acceptInput, inputPosition);
      break;
    case 1: // bitmap
      drawBitmap(screen);
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
  // Graphics::I()->setColor(colors[currentColors[1]][0],
  //                         colors[currentColors[1]][1],
  //                         colors[currentColors[1]][2], alpha);
  // Graphics::I()->rectangle(offset.x,
  //                          offset.y,
  //                          TERMINAL_SCREEN_WIDTH,
  //                          TERMINAL_SCREEN_HEIGHT);
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

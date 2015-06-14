#include "terminal.h"

Terminal::Terminal() {
  offset = Graphics::I()->getScreenScaledOffset();
  std::srand(time(NULL));
}

Terminal::~Terminal() {}

void Terminal::update() {
  if (!isActive()) return;
  if (isBooting()) return;
  if (isSleeping()) return;

  input();
}

void Terminal::draw(int alpha) {
  if (!isActive()) return;

  loadColors();
  drawBackground(alpha);
  drawScreen(alpha);
}

//=============================================================================

void Terminal::input() {
  if (memory[SCREEN_MODE] == 0)
    shellInput();
  else
    ;
}

void Terminal::shellInput() {
  bool* keys = Keyboard::I()->getShellKeys();
  bool* mods = Keyboard::I()->getModKeys();
  uint8_t ch;

  int nextAddr = memory[INPUT_COL] < 39 ?
    memory[INPUT_COL] + 1 : 39;
  int prevAddr = memory[INPUT_COL] > 0 ?
    memory[INPUT_COL] - 1 : 0;

  for (int i = 0; i < 0x80; i++) {
    if (keys[i]) {
      if (i == 8) {
        ch = inputGetCharacter();
        if (ch != 0) inputPutCharacter(0);
        memory[INPUT_COL] = prevAddr;
        inputPutCharacter(ch);
      } else
      if (i == 32 || i >= 48 && i <= 57 && !mods[0]) {
        inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i >= 97 && i <= 122) {
        inputPutCharacter(i - 32);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 91 || i == 93 || i == 92) {
        if (mods[0]) inputPutCharacter(i + 32);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 44 || i == 46 || i == 47) {
        if (mods[0]) inputPutCharacter(i + 16);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 39) {
        if (mods[0]) inputPutCharacter(i - 5);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 59) {
        if (mods[0]) inputPutCharacter(i - 1);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 45) {
        if (mods[0]) inputPutCharacter(i + 50);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 61) {
        if (mods[0]) inputPutCharacter(i - 18);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 96) {
        if (mods[0]) inputPutCharacter(i + 30);
        else         inputPutCharacter(i);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 48) {
        inputPutCharacter(41);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 49) {
        inputPutCharacter(33);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 50) {
        inputPutCharacter(64);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i >= 51 && i <= 53) {
        inputPutCharacter(i - 16);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 54) {
        inputPutCharacter(94);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 55) {
        inputPutCharacter(38);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 56) {
        inputPutCharacter(42);
        memory[INPUT_COL] = nextAddr;
      } else
      if (i == 57) {
        inputPutCharacter(40);
        memory[INPUT_COL] = nextAddr;
      }
    }
  }
}

uint8_t Terminal::inputGetCharacter() {
  p = SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
  return memory[p];
}

void Terminal::inputPutCharacter(uint8_t value) {
  p = SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
  memory[p] = value;
}

//=============================================================================

void Terminal::loadColors() {
  int p = COLORS_ADDRESS;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 3; j++) {
      colors[i][j] = memory[p++];
    }
  }
}

void Terminal::drawBackground(int alpha) {
  fg = getRight(memory[CURRENT_COLORS]);
  bg = getLeft(memory[CURRENT_COLORS]);

  Graphics::I()->setColor(colors[fg][0],
                          colors[fg][1],
                          colors[fg][2], alpha);
  Graphics::I()->rectangle(offset.x - margin,
                           offset.y - margin,
                           TERMINAL_SCREEN_WIDTH + margin * 2,
                           TERMINAL_SCREEN_HEIGHT + margin * 2);
  Graphics::I()->setColor(colors[bg][0],
                          colors[bg][1],
                          colors[bg][2], alpha);
  Graphics::I()->rectangle(offset.x,
                           offset.y,
                           TERMINAL_SCREEN_WIDTH,
                           TERMINAL_SCREEN_HEIGHT);
}

void Terminal::drawScreen(int alpha) {
  p = SCREEN_ADDRESS;
  if (inShell())
    drawShellMode(alpha);
  else
    drawBitmapMode(alpha);
}

void Terminal::drawShellMode(int alpha) {
  uint8_t ch;
  int _p, x, y, bit, col;
  x = 0;
  y = 0;
  while (p < SHELL_SCREEN_END) {
    ch = memory[p];
    _p = FONT_ADDRESS + ch * 8;

    for (int j = 0; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        bit = getBitAt(memory[_p + j], 7 - i);

        if (bit == 1)
          col = fg;
        else
          col = bg;

        if (memory[ACTIVE_INPUT] != 0)
          if (p == SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL])
            if (col == fg) col = bg;
            else           col = fg;

        Graphics::I()->setColor(colors[col][0], colors[col][1], colors[col][2], alpha);
        Graphics::I()->point(x + i + offset.x,
                             y + j + offset.y);
      }
    }

    x += 8;
    if (x >= 320) {
      x = 0;
      y += 8;
    }
    p++;
  }
}

void Terminal::drawBitmapMode(int alpha) {
  uint8_t bit;
  bool left = true;

  for (int j = 0; j < TERMINAL_SCREEN_HEIGHT; j++) {
    for (int i = 0; i < TERMINAL_SCREEN_WIDTH; i++) {
      if (left) { // 0xF0 & bit
        bit = getLeft(memory[p]);
      } else {    // 0x0F & bit
        bit = getRight(memory[p]);
        p++;
      }
      left = !left;

      Graphics::I()->setColor(colors[bit][0], colors[bit][1], colors[bit][2], alpha);
      Graphics::I()->point(i + offset.x, j + offset.y);
    }
  }
}

//=============================================================================

uint8_t Terminal::randomRange(int low, int high) {
  uint8_t result = (std::rand() % (high - low)) + low;
  return result;
}

uint8_t Terminal::getBitAt(uint8_t val, uint8_t pos) {
  return (val >> pos) & 0x1;
}

uint8_t Terminal::getLeft(uint8_t val) {
  uint8_t r = val >> 4;
  return r;
}

uint8_t Terminal::getRight(uint8_t val) {
  uint8_t r = val << 4;
  r = r >> 4;
  return r;
}

//=============================================================================

void Terminal::turnOn() {
  reset();
  active = true;
  uptime = 0;
}

void Terminal::turnOff() {
  active = false;
}

bool Terminal::isActive() {
  return active;
}

bool Terminal::isBooting() {
  if (uptime >= bootTime) return false;

  uptime += Graphics::I()->dt();
  if (uptime >= bootTime)
    resetShell();

  return true;
}

bool Terminal::isSleeping() {
  if (sleep <= 0) return false;

  sleep -= Graphics::I()->dt();
  if (sleep <= 0)
    readyInput();

  return true;
}

//=============================================================================

bool Terminal::inShell() {
  return memory[SCREEN_MODE] == 0;
}

bool Terminal::inBitmap() {
  return !inShell();
}

//=============================================================================

void Terminal::reset() {
  for (int i = 0; i < TERMINAL_MEMORY; i++)
    memory[i] = 0;

  p = FONT_ADDRESS;
  for (int i = 0; i < (sizeof(originalFont)/sizeof(*originalFont)); i++)
    memory[p++] = originalFont[i];

  p = CURRENT_COLORS;
  memory[p++] = originalColor;

  p = COLORS_ADDRESS;
  for (int i = 0; i < 16; i++)
    for (int j = 0; j < 3; j++)
      memory[p++] = originalColors[i][j];

  p = SCREEN_MODE;
  memory[p] = 0;

  // DEBUG SCREEN CONTENT
  // for (int i = SCREEN_ADDRESS; i < CURRENT_COLORS; i++) {
    // memory[i] = 64 + i - SCREEN_ADDRESS;
    // memory[i] = 0;
  // }
}

void Terminal::resetShell() {
  unsigned int c = 0;
  const uint8_t lines = 5;
  for (int i = SCREEN_ADDRESS; i < SCREEN_ADDRESS + SHELL_WIDTH * lines; i++)
    memory[i] = greetText[c++];

  memory[INPUT_ROW] = lines;
  memory[INPUT_COL] = 0;

  readyBeforeInput();
}

void Terminal::readyBeforeInput() {
  sleep = inputSleep;
}

void Terminal::readyInput() {
  p = SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
  for (int i = 0; i < 40; i++)
    memory[p++] = newInputLine[i];

  memory[INPUT_ROW]++;
  memory[ACTIVE_INPUT] = 1;
}

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
  if (memory[ACTIVE_INPUT] == 0) return;

  shellInputKeys* keys = Keyboard::I()->getShellKeys();

  int nextAddr = memory[INPUT_COL] < 39 ?
    memory[INPUT_COL] + 1 : 39;
  int prevAddr = memory[INPUT_COL] > 0 ?
    memory[INPUT_COL] - 1 : 0;

  if (keys->Backspace) {
    if (inputGetCurrentPosition() > inputGetStartOfLine()) {
      p  = inputGetStartOfLine() + prevAddr;
      _p = inputGetEndOfLine();
      for (uint16_t j = p; j < _p; j++)
        memory[j] = memory[j + 1];
      memory[_p] = 0;
      memory[INPUT_COL] = prevAddr;
    }
  }
  if (keys->Delete) {
    p  = inputGetCurrentPosition();
    _p = inputGetEndOfLine();
    for (uint16_t j = p; j < _p; j++)
      memory[j] = memory[j + 1];
    memory[_p] = 0;
  }
  if (keys->Enter) {
    processInput();
    return;
  }

  for (int i = 32; i <= 126; i++) {
    if (keys->text[i]) {
      inputPutCharacter(
        Keyboard::I()->getShellInputKey(i)
      );
      memory[INPUT_COL] = nextAddr;
    }
  }

  if (keys->Left) {
    memory[INPUT_COL] = prevAddr;
  } else
  if (keys->Right) {
    memory[INPUT_COL] = nextAddr;
  }
}

uint16_t Terminal::inputGetCurrentPosition() {
  return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
}

uint16_t Terminal::inputGetStartOfLine() {
  return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH;
}

uint16_t Terminal::inputGetEndOfLine() {
  return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + SHELL_WIDTH - 1;
}

uint8_t Terminal::inputGetCharacter() {
  p = inputGetCurrentPosition();
  return memory[p];
}

void Terminal::inputPutCharacter(uint8_t value) {
  p = inputGetCurrentPosition();
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
  shellNextLine();

  p = SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
  for (int i = 0; i < 40; i++)
    memory[p++] = newInputLine[i];

  shellNextLine();
  memory[ACTIVE_INPUT] = 1;
}

void Terminal::processInput() {
  memory[ACTIVE_INPUT] = 0;
  p = inputGetStartOfLine();
  command = "";
  for (int i = 0; i < SHELL_WIDTH; i++) {
    if (memory[p] == 0) break;
    command += memory[p++];
  }

  shellNextLine();
  processCommand();
}

void Terminal::shellNextLine() {
  if (memory[INPUT_ROW] == SHELL_HEIGHT - 1) {
    p = SCREEN_ADDRESS + SHELL_WIDTH;
    _p = SHELL_SCREEN_END - SHELL_WIDTH;
    for (uint16_t i = p; i < _p; i++)
      memory[i] = memory[i + SHELL_WIDTH];
    p = SHELL_SCREEN_END - SHELL_WIDTH;
    _p = SHELL_SCREEN_END;
    for (uint16_t i = p; i < _p; i++)
      memory[i] = 0;
  } else {
    memory[INPUT_ROW]++;
  }
  memory[INPUT_COL] = 0;
}

//=============================================================================

void Terminal::processCommand() {
  std::string args[SHELL_WIDTH / 2];
  int c = 0;
  for (int i = 0; i < command.length(); i++) {
    if (command[i] == ' ')
      c++;
    else
      args[c] += command[i];
  }
  c++;

  if (args[0] == "RUN") {
    output = "CAN'T RUN SHIT FOR NOW";
  } else
  if (args[0] == "LOAD") {
    output = "DOESN'T WORK";
  } else
  if (args[0] == "EDIT") {
    output = "NOT IMPLEMENTED";
  } else {
    output = "SYNTAX ERROR";
  }

  shellPrintOutput();
}

void Terminal::shellPrintOutput() {
  p = inputGetStartOfLine();
  for (int i = 0; i < output.length(); i++) {
    memory[p++] = output[i];
  }

  shellNextLine();
  readyBeforeInput();
}

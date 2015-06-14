#include "memory.h"

Memory::Memory() {}
Memory::~Memory() {}

//=============================================================================

uint8_t Memory::screenMode() {
  if (m[SCREEN_MODE] == 0)
    return SHELL_MODE;
  else
    return BITMAP_MODE;
}

colorMap Memory::getColors() {
  p = COLORS;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 3; j++) {
      colors[i][j] = m[p++];
    }
  }
  return colors;
}

colorBit Memory::getCurrentColors() {
  currentColors[0] = getRightByte(m[CURRENT_COLORS]);
  currentColors[1] = getLeftByte(m[CURRENT_COLORS]);
  return currentColors;
}

uint8_t* Memory::fontAddress() {
  return &m[FONT];
}

uint8_t* Memory::screenAddress() {
  return &m[SCREEN_ADDRESS];
}

Position Memory::inputPosition() {
  Position pos;
  pos.x = m[INPUT_COL];
  pos.y = m[INPUT_ROW];
  return pos;
}

//=============================================================================

bool Memory::isInShell() {
  return screenMode() == SHELL_MODE;
}

bool Memory::isInBitmap() {
  return screenMode() == BITMAP_MODE;
}

bool Memory::isAcceptInput() {
  return isInShell() && m[ACCEPT_INPUT] == 1;
}

//=============================================================================

void Memory::buildDefaultRAM() {
  for (int i = 0; i < MEMORY_SIZE; i++)
    m[i] = 0;

  p = FONT;
  for (int i = 0; i < (sizeof(originalFont)/sizeof(*originalFont)); i++)
    m[p++] = originalFont[i];

  p = CURRENT_COLORS;
  m[p++] = originalColor;

  p = COLORS;
  for (int i = 0; i < 16; i++)
    for (int j = 0; j < 3; j++)
      m[p++] = originalColors[i][j];

  p = SCREEN_MODE;
  m[p] = 0;
}

void Memory::reboot() {
  clearScreen();
  printText(greetText, SCREEN_ADDRESS);
}

//=============================================================================

void Memory::acceptInput() {
  printText(newInputLine);
  m[ACCEPT_INPUT] = 1;
}

void Memory::denyInput() {
  m[ACCEPT_INPUT] = 0;
}

//=============================================================================

uint16_t Memory::currentInputAddress() {
  return SCREEN_ADDRESS + m[INPUT_ROW] * SHELL_WIDTH + m[INPUT_COL];
}

uint16_t Memory::currentInputLineAddress() {
  return SCREEN_ADDRESS + m[INPUT_ROW] * SHELL_WIDTH;
}

//=============================================================================

void Memory::clearScreen() {
  p  = SCREEN_ADDRESS;
  _p = isInBitmap() ? BITMAP_MODE_END : SHELL_MODE_END;
  while (p < _p) {
    m[p++] = 0;
  }
}

void Memory::printText(std::string text) {
  printText(text, currentInputAddress());
}

void Memory::printLine(std::string text) {
  printText(text, currentInputLineAddress());
}

void Memory::printText(std::string text, uint16_t address) {
  p = address;
  for (int i = 0; i < text.length(); i++) {
    m[p++] = text[i];
  }

  newLine((text.length() + SHELL_WIDTH - 1) / SHELL_WIDTH);
}

void Memory::newLine() {
  newLine(1);
}

void Memory::newLine(int lines) {
  for (int i = 0; i < lines; i++) {
    m[INPUT_ROW]++;
  }
  m[INPUT_COL] = 0;
}


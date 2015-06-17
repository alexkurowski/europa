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
  printText(greetText, static_cast<uint16_t>(SCREEN_ADDRESS));
}

//=============================================================================

void Memory::acceptInput() {
  newLine();
  printText(newInputLine, false);
  m[ACCEPT_INPUT] = 1;
}

void Memory::denyInput() {
  m[ACCEPT_INPUT] = 0;
}

void Memory::setInputBits(bool* keys) {
  p = KEYBOARD_KEYS;
  uint8_t bit = 0;
  uint8_t byte = 0;
  for (uint8_t i = 0; i < 128; i++) {
    if (keys[i])
      byte += (1 << 7 - bit);

    bit++;
    if (bit < 8) continue;

    m[p] = byte;

    byte = 0;
    bit  = 0;
    p++;
  }
}

//=============================================================================

uint16_t Memory::currentInputAddress() {
  return SCREEN_ADDRESS + m[INPUT_ROW] * SHELL_WIDTH + m[INPUT_COL];
}

uint16_t Memory::currentInputLineAddress() {
  return SCREEN_ADDRESS + m[INPUT_ROW] * SHELL_WIDTH;
}

//=============================================================================

void Memory::setByte(uint16_t addr, uint8_t val) {
  m[addr] = val;
}

void Memory::setByte(uint8_t val) {
  m[getAddress()] = val;
}

uint8_t Memory::getByte(uint16_t addr) {
  return m[addr];
}

uint8_t Memory::getByte() {
  return m[getAddress()];
}

void Memory::setAddress(uint16_t value) {
  m[CURRENT_ADDRESS]     = (value & 0xFF00) >> 8;
  m[CURRENT_ADDRESS + 1] = (value & 0x00FF);
}

uint16_t Memory::getAddress() {
  return (m[CURRENT_ADDRESS] << 8) + m[CURRENT_ADDRESS + 1];
}

uint8_t Memory::getBit(uint16_t addr, uint8_t pos) {
  return getBitAt(m[addr], 7 - pos % 8);
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
  printText(text, true);
}

void Memory::printText(std::string text, bool newline) {
  if (newline) newLine();
  printText(text, currentInputAddress());
}

void Memory::printLine(std::string text) {
  printText(text, currentInputLineAddress());
}

void Memory::printText(std::string text, uint16_t address) {
  uint8_t lines = (text.length() + SHELL_WIDTH - 1) / SHELL_WIDTH;

  uint8_t addedLines = newLine(lines - 1);
  p = address;

  if (addedLines > 0) {
    p -= SHELL_WIDTH * addedLines;
  }

  for (int i = 0; i < text.length(); i++) {
    m[p++] = text[i];
  }

  newLine();
}

void Memory::printChar(char ch) {
  printChar(ch, currentInputAddress());
}

void Memory::printChar(char ch, uint16_t addr) {
  m[addr] = ch;
}

void Memory::newLine() {
  newLine(1);
}

int Memory::newLine(int lines) {
  int shiftedLines = 0;

  for (int i = 0; i < lines; i++) {
    if (m[INPUT_ROW] < SHELL_HEIGHT - 1) {
      m[INPUT_ROW]++;
    } else {
      shiftTextUp();
      shiftedLines++;
    }
  }

  m[INPUT_COL] = 0;
  return shiftedLines;
}

void Memory::shiftTextUp() {
  p  = SCREEN_ADDRESS;
  _p = SHELL_MODE_END - SHELL_WIDTH;

  for (uint16_t i = p; i < _p; i++)
    m[i] = m[i + SHELL_WIDTH];

  for (uint16_t i = _p; i < SHELL_MODE_END; i++)
    m[i] = 0;
}


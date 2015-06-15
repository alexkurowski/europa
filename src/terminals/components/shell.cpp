#include "shell.h"

Shell::Shell(Memory* m) {
  mem = m;
}

Shell::~Shell() {}

void Shell::update() {
  if (mem->isAcceptInput())
    input();
}

//=============================================================================

void Shell::input() {
  for (uint8_t i = CHAR_TEXT_START; i <= CHAR_TEXT_END; i++) {
    if (isKeyDown(i)) {
      mem->printChar(i);
      moveCaretRight();
    }
  }

  if (isKeyDown(CHAR_ARROW_LEFT))
    moveCaretLeft();

  if (isKeyDown(CHAR_ARROW_RIGHT))
    moveCaretRight();

  if (isKeyDown(CHAR_BACKSPACE)) {
    moveCaretLeft();
    mem->printChar(0);
    // move characters from caret till the end of line to the left
  }

  if (isKeyDown(CHAR_DELETE)) {
    mem->printChar(0);
    // move characters from caret till the end of line to the left
  }
}

bool Shell::isKeyDown(char i) {
  return mem->getBit(KEYBOARD_KEYS + (int)(i / 8), i) == 1;
}

Position Shell::caret() {
  Position pos;
  pos.x = mem->getByte(INPUT_COL);
  pos.y = mem->getByte(INPUT_ROW);
  return pos;
}

void Shell::moveCaretLeft() {
  t = mem->getByte(INPUT_COL);
  if (t > 0)
    mem->setByte(INPUT_COL, t - 1);
}

void Shell::moveCaretRight() {
  t = mem->getByte(INPUT_COL);
  if (t < SHELL_WIDTH - 1)
    mem->setByte(INPUT_COL, t + 1);
}

//=============================================================================


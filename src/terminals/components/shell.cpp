#include "shell.h"

Shell::Shell(Memory* m) {
  mem  = m;
}

Shell::~Shell() {}

void Shell::update() {
  before();

  if (mem->isAcceptInput())
    input();
}

void Shell::before() {
  if (args.size() > 0)
    args.clear();
  if (messageToTerminal != "")
    messageToTerminal = "";
}

//=============================================================================

void Shell::input() {
  for (uint8_t i = CHAR_TEXT_START; i <= CHAR_TEXT_END; i++) {
    if (isKeyDown(i)) {
      moveInputLineRight();
      mem->printChar(i);
      moveCaretRight();
    }
  }

  if (isKeyDown(CHAR_ARROW_LEFT))
    moveCaretLeft();

  if (isKeyDown(CHAR_ARROW_RIGHT))
    moveCaretRight();

  if (isKeyDown(CHAR_BACKSPACE)) {
    if (mem->currentInputAddress() > mem->currentInputLineAddress()) {
      moveCaretLeft();
      mem->printChar(0);
      moveInputLineLeft();
    }
  }

  if (isKeyDown(CHAR_DELETE)) {
    mem->printChar(0);
    moveInputLineLeft();
  }

  if (isKeyDown(CHAR_ENTER)) {
    setArgs();
    submit();
  }
}

bool Shell::isKeyDown(char i) {
  return mem->getBit(KEYBOARD_KEYS + (int)(i / 8), i) == 1;
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

void Shell::moveInputLineLeft() {
  for (p = mem->currentInputAddress() + 1; p < mem->currentInputLineAddress() + SHELL_WIDTH; p++) {
    mem->setByte(p - 1, mem->getByte(p));
  }
  mem->setByte(mem->currentInputLineAddress() + SHELL_WIDTH - 1, 0);
}

void Shell::moveInputLineRight() {
  for (p = mem->currentInputLineAddress() + SHELL_WIDTH - 2; p > mem->currentInputAddress() - 1; p--) {
    mem->setByte(p + 1, mem->getByte(p));
  }
}

//=============================================================================

void Shell::setArgs() {
  args.push_back("");

  uint8_t c = 0;
  char ch;
  for (p = mem->currentInputLineAddress(); p < mem->currentInputLineAddress() + SHELL_WIDTH; p++) {
    ch = mem->getByte(p);
    if (ch == 32) {
      if (args[c] != "") {
        c++;
        args.push_back("");
      }
      continue;
    }
    if (ch >= CHAR_TEXT_START && ch <= CHAR_TEXT_END) {
      args[c] += ch;
    }
  }

  if (args[0] == "")
    args.clear();
}

void Shell::submit() {
  if (args.size() == 0 || args[0] == "")
    return;

  if (args[0] == "REBOOT") {
    messageToTerminal = "reboot";
  } else
  if (args[0] == "HELP") {
    mem->printText(helpText);
  } else
  if (args[0] == "LIST") {
    mem->printText(listText);
  } else {
    mem->printText(errorText);
  }

  if (messageToTerminal == "")
    messageToTerminal = "new line";
}

std::string Shell::getMessage() {
  return messageToTerminal;
}

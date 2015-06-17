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
  std::string c = args[0];
  if (args.size() == 0 || c == "")
    return;

  if (c == "RUN")                 commandRun(args[1]);
  else
  if (c == "EDIT")                commandEdit(args[1]);
  else
  if (c == "REBOOT")              commandReboot();
  else
  if (c == "QUIT" || c == "EXIT") commandQuit();
  else
  if (c == "HELP" || c == "?")    commandHelp();
  else
  if (c == "LIST")                commandList();
  else
  if (c == "CLEAR")               commandClear();
  else
                                  commandError();

  commandDefault();
}

void Shell::commandRun(std::string name) {
  for (int i = 0; i < name.length(); i++) {
    name[i] = std::tolower(name[i]);
  }
  messageToTerminal = "run " + name; //std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  // if (name == "REMOTE") messageToTerminal = "run remote";
  // if (name == "MAIL")   messageToTerminal = "run mail";
}

void Shell::commandEdit(std::string name) {
}

void Shell::commandList() {
  std::string files = " ";

  tinydir_dir dir;
  tinydir_open(&dir, "./asm");

  while (dir.has_next) {
    tinydir_file file;
    tinydir_readfile(&dir, &file);

    if (!file.is_dir &&
        file.name[0] != '.') {
      for (int i = 0; i < sizeof(file.name); i++) {
        file.name[i] = std::toupper(file.name[i]);
      }
      files += file.name;
      files += "   ";
    }

    tinydir_next(&dir);
  }
  tinydir_close(&dir);

  mem->printText(files);
}

void Shell::commandHelp() {
  mem->printText(helpText);
}

void Shell::commandClear() {
  mem->clearScreen();
  mem->setByte(INPUT_ROW, 0);
  mem->setByte(INPUT_COL, 0);
}

void Shell::commandReboot() {
  messageToTerminal = "reboot";
}

void Shell::commandQuit() {
  messageToTerminal = "quit";
}

void Shell::commandError() {
  mem->printText(errorText);
}

void Shell::commandDefault() {
  if (messageToTerminal == "")
    messageToTerminal = "new line";
}

std::string Shell::getMessage() {
  return messageToTerminal;
}

#include "terminal.h"

Terminal::Terminal() {
  mem     = new Memory();
  display = new Display();

  std::srand(time(NULL));
}

Terminal::~Terminal() {
  delete mem;
  delete display;
}

void Terminal::update() {
  if (!isActive())  return;
  if (isSleeping()) return;

  // input();
}

void Terminal::draw(uint8_t alpha) {
  if (!isActive()) return;

  display->draw(mem->screenMode(),
                mem->getColors(),
                mem->getCurrentColors(),
                mem->fontAddress(),
                mem->screenAddress(),
                mem->isAcceptInput(),
                mem->inputPosition(),
                alpha);
}

//=============================================================================

void Terminal::turnOn() {
  active = true;
  mem->buildDefaultRAM();
  setSleep(bootTime, &Terminal::afterBoot);
}

void Terminal::turnOff() {
  active = false;
}

bool Terminal::isActive() {
  return active;
}

//=============================================================================

void Terminal::afterBoot() {
  mem->reboot();
  setSleep(inputDelay, &Terminal::beforeInput);
}

void Terminal::beforeInput() {
  mem->acceptInput();
}

//=============================================================================

void Terminal::setSleep(float time, void (Terminal::*fn)()) {
  sleep = time;

  afterSleep   = fn;
  doAfterSleep = true;
}

void Terminal::setSleep(float time) {
  sleep = time;

  doAfterSleep = false;
}

bool Terminal::isSleeping() {
  if (sleep <= 0) return false;

  sleep -= Graphics::I()->dt();
  if (sleep <= 0 && doAfterSleep) {
    doAfterSleep = false;
    (*this.*afterSleep)();
  }

  return true;
}

//=============================================================================

// void Terminal::input() {
//   if (memory[SCREEN_MODE] == 0)
//     shellInput();
//   else
//     ;
// }
//
// void Terminal::shellInput() {
//   if (memory[ACTIVE_INPUT] == 0) return;
//
//   shellInputKeys* keys = Keyboard::I()->getShellKeys();
//
//   int nextAddr = memory[INPUT_COL] < 39 ?
//     memory[INPUT_COL] + 1 : 39;
//   int prevAddr = memory[INPUT_COL] > 0 ?
//     memory[INPUT_COL] - 1 : 0;
//
//   if (keys->Backspace) {
//     if (inputGetCurrentPosition() > inputGetStartOfLine()) {
//       p  = inputGetStartOfLine() + prevAddr;
//       _p = inputGetEndOfLine();
//       for (uint16_t j = p; j < _p; j++)
//         memory[j] = memory[j + 1];
//       memory[_p] = 0;
//       memory[INPUT_COL] = prevAddr;
//     }
//   }
//   if (keys->Delete) {
//     p  = inputGetCurrentPosition();
//     _p = inputGetEndOfLine();
//     for (uint16_t j = p; j < _p; j++)
//       memory[j] = memory[j + 1];
//     memory[_p] = 0;
//   }
//   if (keys->Enter) {
//     processInput();
//     return;
//   }
//
//   for (int i = 32; i <= 126; i++) {
//     if (keys->text[i]) {
//       inputPutCharacter(
//         Keyboard::I()->getShellInputKey(i)
//       );
//       memory[INPUT_COL] = nextAddr;
//     }
//   }
//
//   if (keys->Left) {
//     memory[INPUT_COL] = prevAddr;
//   } else
//   if (keys->Right) {
//     memory[INPUT_COL] = nextAddr;
//   }
// }
//
// uint16_t Terminal::inputGetCurrentPosition() {
//   return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
// }
//
// uint16_t Terminal::inputGetStartOfLine() {
//   return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH;
// }
//
// uint16_t Terminal::inputGetEndOfLine() {
//   return SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + SHELL_WIDTH - 1;
// }
//
// uint8_t Terminal::inputGetCharacter() {
//   p = inputGetCurrentPosition();
//   return memory[p];
// }
//
// void Terminal::inputPutCharacter(uint8_t value) {
//   p = inputGetCurrentPosition();
//   memory[p] = value;
// }
//
// //=============================================================================
//
// uint8_t Terminal::randomRange(int low, int high) {
//   uint8_t result = (std::rand() % (high - low)) + low;
//   return result;
// }
//
// //=============================================================================
//

// //=============================================================================
//
// bool Terminal::inShell() {
//   return memory[SCREEN_MODE] == 0;
// }
//
// bool Terminal::inBitmap() {
//   return !inShell();
// }
//
// //=============================================================================
//
//
// void Terminal::resetShell() {
//   unsigned int c = 0;
//   const uint8_t lines = 5;
//   for (int i = SCREEN_ADDRESS; i < SCREEN_ADDRESS + SHELL_WIDTH * lines; i++)
//     memory[i] = greetText[c++];
//
//   memory[INPUT_ROW] = lines;
//   memory[INPUT_COL] = 0;
//
//   readyBeforeInput();
// }
//
// void Terminal::readyBeforeInput() {
//   sleep = inputSleep;
// }
//
// void Terminal::readyInput() {
//   shellNextLine();
//
//   p = SCREEN_ADDRESS + memory[INPUT_ROW] * SHELL_WIDTH + memory[INPUT_COL];
//   for (int i = 0; i < 40; i++)
//     memory[p++] = newInputLine[i];
//
//   shellNextLine();
//   memory[ACTIVE_INPUT] = 1;
// }
//
// void Terminal::processInput() {
//   memory[ACTIVE_INPUT] = 0;
//   p = inputGetStartOfLine();
//   command = "";
//   for (int i = 0; i < SHELL_WIDTH; i++) {
//     if (memory[p] == 0) break;
//     command += memory[p++];
//   }
//
//   shellNextLine();
//   processCommand();
// }
//
// void Terminal::shellNextLine() {
//   if (memory[INPUT_ROW] == SHELL_HEIGHT - 1) {
//     p = SCREEN_ADDRESS + SHELL_WIDTH;
//     _p = SHELL_SCREEN_END - SHELL_WIDTH;
//     for (uint16_t i = p; i < _p; i++)
//       memory[i] = memory[i + SHELL_WIDTH];
//     p = SHELL_SCREEN_END - SHELL_WIDTH;
//     _p = SHELL_SCREEN_END;
//     for (uint16_t i = p; i < _p; i++)
//       memory[i] = 0;
//   } else {
//     memory[INPUT_ROW]++;
//   }
//   memory[INPUT_COL] = 0;
// }
//
// //=============================================================================
//
// void Terminal::processCommand() {
//   std::string args[SHELL_WIDTH / 2];
//   int c = 0;
//   for (int i = 0; i < command.length(); i++) {
//     if (command[i] == ' ')
//       c++;
//     else
//       args[c] += command[i];
//   }
//   c++;
//
//   if (args[0] == "RUN") {
//     output = "CAN'T RUN SHIT FOR NOW";
//   } else
//   if (args[0] == "LOAD") {
//     output = "DOESN'T WORK";
//   } else
//   if (args[0] == "HELP") {
//     shellCommandHelp();
//   } else
//   if (args[0] == "EDIT") {
//     output = "NOT IMPLEMENTED";
//   } else {
//     output = "SYNTAX ERROR";
//     shellPrintOutput();
//   }
// }
//
// void Terminal::shellPrintOutput() {
//   p = inputGetStartOfLine();
//   for (int i = 0; i < output.length(); i++) {
//     memory[p++] = output[i];
//   }
//
//   shellNextLine();
//   readyBeforeInput();
// }
//
// void Terminal::shellCommandRun() {
// }
//
// void Terminal::shellCommandEdit() {
// }
//
// void Terminal::shellCommandList() {
// }
//
// void Terminal::shellCommandHelp() {
//   output = helpText;
//   shellPrintOutput();
//
//   shellNextLine();
//   shellNextLine();
//   shellNextLine();
//   shellNextLine();
//   shellNextLine();
// }

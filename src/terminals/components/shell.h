#pragma once

#include <stdint.h>
#include <vector>
#include <algorithm>
#include <string>
#include "memory.h"
#include "../../../lib/tinydir.h"
#include "../../../lib/keyboard.h"

class Shell {
  public:
    Shell(Memory*);
    ~Shell();

    void update();

    std::string getMessage();

  private:
    Memory*   mem;

    uint8_t  t;
    uint16_t p;

    std::vector<std::string> args;

    std::string messageToTerminal;

    void before();

    void input();
    void setArgs();
    void submit();


    bool isKeyDown(char);

    void moveCaretLeft();
    void moveCaretRight();
    void moveInputLineLeft();
    void moveInputLineRight();


    void commandRun(std::string);
    void commandEdit(std::string);
    void commandList();
    void commandHelp();
    void commandClear();
    void commandReboot();
    void commandQuit();
    void commandError();
    void commandDefault();


    const std::string errorText = "?SYNTAX ERROR                           ";

    const std::string helpText  = " LIST      - DISPLAY NAMES OF AVAILABLE "
                                  "             PROGRAMS                   "
                                  " RUN NAME  - RUN PROGRAM WITH A GIVEN   "
                                  "             NAME                       "
                                  " EDIT NAME - LOAD PROGRAM WITH A GIVEN  "
                                  "             NAME AND OPEN EDITOR       "
                                  " REBOOT    - RESTART THE TERMINAL AND   "
                                  "             RESET MEMORY               "
                                  " CLEAR     - CLEAR THE TERMINAL SCREEN  ";
};

#pragma once

#include <stdint.h>
#include "memory.h"
#include "../../../lib/keyboard.h"

class Shell {
  public:
    Shell(Memory*);
    ~Shell();

    void update();

  private:
    Memory* mem;

    uint8_t  t;
    uint16_t p;

    void input();

    bool isKeyDown(char);

    Position caret();
    void moveCaretLeft();
    void moveCaretRight();
};

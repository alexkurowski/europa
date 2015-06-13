#pragma once

#include "character.h"
#include "../../lib/graphics.h"
#include "../../lib/data.h"
#include "../terminals/terminal.h"

class Base {
  public:
    Base();
    ~Base();

    void addTerminal(Terminal*);
    int currentTerminal();

    void update();
    void draw();

    void moveTo(Position pos);

  private:
    Character* character;

    Texture background;

    int terminalCount = 0;
    Texture* terminals[TERMINAL_COUNT];
    int _currentTerminal = -1;

    void alignToTerminal(Position*);
};

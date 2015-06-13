#pragma once

#include <iostream>
#include "state.h"
#include "../base/base.h"
#include "../screen/screen.h"
#include "../terminals/terminal.h"
#include "../../lib/data.h"

class Play : public State {
  public:
    Play();
    ~Play();

    void update() override;
    void draw() override;

    void leftClick(Position pos) override;

  private:
    Base* base;
    Screen* screen;

    int currentTerminal = -1;
    int nextTerminal = -1;
};

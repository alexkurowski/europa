#pragma once

#include <iostream>
#include "state.h"
#include "../base/base.h"
#include "../terminals/terminal.h"

const int TERMINAL_COUNT = 4;

class Play : public State {
  public:
    Play();
    ~Play();

    void update() override;
    void draw() override;

    void leftClick(Position pos) override;

  private:
    Base* base;
    Terminal* terminals[TERMINAL_COUNT];

};

#pragma once

#include "../terminals/terminal.h"
#include "../../lib/data.h"
#include "../../lib/graphics.h"

class Screen {
  public:
    Screen();
    ~Screen();

    void addTerminal(Terminal*);

    void update();
    void draw();

    void setNextTerminal(int);
    Terminal* terminal();

  private:
    int fade = 0;
    const int fadeSpeed = 600;
    SDL_Rect fadeRect;

    int currentTerminal = -1;
    int nextTerminal = -1;

    int termCount = 0;
    Terminal* terminals[TERMINAL_COUNT];

    void updateFade();
    void fadeIn();
    void fadeOut();
    void fadeAway();
};

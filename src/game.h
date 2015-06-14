#pragma once

#include <SDL2/SDL.h>
#include "states/state.h"
#include "states/play.h"
#include "../lib/graphics.h"
#include "../lib/keyboard.h"

class Game {
  public:
    Game();
    ~Game();

    bool active = true;

    void input();
    void update();
    void draw();

  private:
    SDL_Event event;

    unsigned int frame = 0;
    unsigned int sec = 0;

    State* state;

    void printFPS();
};

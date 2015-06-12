#pragma once

#include <SDL2/SDL.h>
#include "states/state.h"
#include "states/play.h"

class Game {
  public:
    Game();
    ~Game();

    bool quit;

    void input();
    void update();
    void draw();

  private:
    SDL_Event event;

    State* state;
};

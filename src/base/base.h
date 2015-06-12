#pragma once

#include "character.h"
#include "../../lib/sdlgraphics.h"

extern SDLGraphics* graphics;

class Base {
  public:
    Base();
    ~Base();

    void update();
    void draw();

  private:
    Character* character;

    SDL_Texture* background;
};

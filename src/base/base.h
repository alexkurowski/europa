#pragma once

#include "character.h"
#include "../../lib/graphics.h"
#include "../../lib/data.h"

class Base {
  public:
    Base();
    ~Base();

    void update();
    void draw();

    void moveTo(Position pos);

  private:
    Character* character;

    Texture background;
};

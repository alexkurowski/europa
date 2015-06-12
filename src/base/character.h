#pragma once

#include "../../lib/graphics.h"
#include "../../lib/data.h"
#include <cmath>

struct Movement {
  Position speed;
  Position target;
  const float maxSpeed = 6;
  const float maxRunSpeed = 2;
  const float acceleration = 1;
  const float deceleration = 0.9;
  const float stopDistance = 20;

  float distanceToTarget(float x, float y) {
     return sqrt((x - target.x) * (x - target.x) +
                 (y - target.y) * (y - target.y));
  }
};

class Character {
  public:
    Character(SDL_Rect*);
    ~Character();

    void update();
    void draw();

    void moveTo(Position);
    void updateMove();

  private:
    Texture sprite;

    Movement move;

    SDL_Rect border;
};

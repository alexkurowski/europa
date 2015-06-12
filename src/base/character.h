#pragma once

#include "../../lib/graphics.h"
#include "../../lib/data.h"
#include <cmath>

struct Movement {
  Position position;
  Position speed;
  Position target;
  bool run = false;
  const float maxSpeed = 160;
  const float maxRunSpeed = 260;
  const float acceleration = 1200;
  const float deceleration = 54;
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
    void keepInside(SDL_Rect*);

  private:
    Texture sprite;

    Movement move;

    SDL_Rect border;

    unsigned int lastClick = 0;
    unsigned int currentTime = 0;
    const int doubleClickTime = 600;
};

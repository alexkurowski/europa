#include "character.h"

Character::Character(SDL_Rect* baseRect) {
  sprite.texture = Graphics::I()->loadTexture("assets/images/character.png");
  Graphics::I()->setSize(&sprite);

  sprite.rect.x = 500;
  sprite.rect.y = 500;

  move.speed.x = 0;
  move.speed.y = 0;
  move.target.x = sprite.rect.x;
  move.target.y = sprite.rect.y;

  border.x = baseRect->x;
  border.y = baseRect->y;
  border.w = baseRect->w;
  border.h = baseRect->h;
}

Character::~Character() {
}

void Character::update() {
  updateMove();
}

void Character::draw() {
  Graphics::I()->draw(sprite.texture, &sprite.rect);
}

//=============================================================================

void Character::moveTo(Position pos) {
  move.target.x = pos.x - sprite.rect.w / 2;
  move.target.y = pos.y - sprite.rect.h;
}

void Character::updateMove() {
  if (move.distanceToTarget(sprite.rect.x, sprite.rect.y) > move.stopDistance) {
    float max = move.maxSpeed;

    // difference between position and target
    float dx = move.target.x - sprite.rect.x;
    float dy = move.target.y - sprite.rect.y;
    float magnitude = sqrt(dx * dx + dy * dy);
    // normalize
    dx = dx / magnitude;
    dy = dy / magnitude;

    move.speed.x += dx * move.acceleration;
    move.speed.y += dy * move.acceleration;

    if (sqrt(move.speed.x * move.speed.x + move.speed.y * move.speed.y) > max) {
      move.speed.x = dx * max;
      move.speed.y = dy * max;
    }
  } else {
    move.speed.x *= move.deceleration;
    move.speed.y *= move.deceleration;
  }

  sprite.rect.x += move.speed.x;
  sprite.rect.y += move.speed.y;
}

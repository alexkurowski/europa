#include "character.h"

Character::Character(SDL_Rect* baseRect) {
  sprite.texture = Graphics::I()->loadTexture("assets/images/character.png");
  Graphics::I()->setSize(&sprite);

  target.texture = Graphics::I()->loadTexture("assets/images/character_target.png");
  Graphics::I()->setSize(&target);

  move.position.x = 500;
  move.position.y = 500;

  move.speed.x = 0;
  move.speed.y = 0;
  move.target.x = move.position.x;
  move.target.y = move.position.y;

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
  if (move.position.y > move.target.y) {
    drawTarget();
    drawCharacter();
  } else {
    drawCharacter();
    drawTarget();
  }

}

//=============================================================================

void Character::drawCharacter() {
  sprite.rect.x = (int)(move.position.x - sprite.rect.w / 2);
  sprite.rect.y = (int)(move.position.y - sprite.rect.h);

  Graphics::I()->draw(sprite.texture, &sprite.rect);
}

void Character::drawTarget() {
  if (!isStanding()) {
    target.rect.x = (int)(move.target.x - target.rect.w / 2);
    target.rect.y = (int)(move.target.y - target.rect.h);

    Graphics::I()->draw(target.texture, &target.rect);
  }
}

//=============================================================================

void Character::moveTo(Position pos) {
  move.target.x = pos.x;
  move.target.y = pos.y;

  currentTime = SDL_GetTicks();
  if (currentTime <= lastClick + doubleClickTime)
    move.run = true;
  else
    move.run = false;
  lastClick = currentTime;
}

void Character::updateMove() {
  if (!isStanding()) {
    float max = move.run ? move.maxRunSpeed : move.maxSpeed;

    // difference between target and position
    float dx = move.target.x - move.position.x;
    float dy = move.target.y - move.position.y;
    float magnitude = sqrt(dx * dx + dy * dy);
    // normalize
    dx = dx / magnitude;
    dy = dy / magnitude;

    move.speed.x += dx * move.acceleration * Graphics::I()->dt();
    move.speed.y += dy * move.acceleration * Graphics::I()->dt();

    magnitude = sqrt(move.speed.x * move.speed.x + move.speed.y * move.speed.y);
    if (magnitude > max) {
      move.speed.x = move.speed.x / magnitude * max;
      move.speed.y = move.speed.y / magnitude * max;
    }
  } else {
    move.speed.x *= move.deceleration * Graphics::I()->dt();
    move.speed.y *= move.deceleration * Graphics::I()->dt();

    move.run = false;
  }

  move.position.x += move.speed.x * Graphics::I()->dt();
  move.position.y += move.speed.y * Graphics::I()->dt();
}

void Character::keepInside(SDL_Rect* rect) {
  if (move.position.x < rect->x)           move.position.x = rect->x;
  if (move.position.x > rect->x + rect->w) move.position.x = rect->x + rect->w;
  if (move.position.y < rect->y)           move.position.y = rect->y;
  if (move.position.y > rect->y + rect->h) move.position.y = rect->y + rect->h;
}

bool Character::isStanding() {
  if (move.distanceToTarget(move.position.x, move.position.y) > move.stopDistance)
    return false;
  return true;
}

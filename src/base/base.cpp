#include "base.h"

Base::Base() {
  background.texture = Graphics::I()->loadTexture("assets/images/base_background.png");
  Graphics::I()->setSize(&background);
  background.rect.x = Graphics::I()->originalWidth() / 2 - background.rect.w / 2;
  background.rect.y = Graphics::I()->originalHeight() / 2 - background.rect.h / 2;

  character = new Character(&background.rect);
}

Base::~Base() {
  delete character;
}

void Base::update() {
  character->update();
  character->keepInside(&background.rect);
}

void Base::draw() {
  Graphics::I()->setColor(255, 255, 255);
  Graphics::I()->draw(background.texture, &background.rect);

  character->draw();
}

//=============================================================================

void Base::moveTo(Position pos) {
  if (pos.x < background.rect.x ||
      pos.x > background.rect.x + background.rect.w ||
      pos.y < background.rect.y ||
      pos.y > background.rect.y + background.rect.h)
    return;

  character->moveTo(pos);
}

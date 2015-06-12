#include "base.h"

Base::Base() {
  character = new Character();

  // background = IMG_Load("../../assets/images/base_background.png");
  background = graphics->loadTexture("assets/images/base_background.png");
}

Base::~Base() {
  delete character;
}

void Base::update() {
}

void Base::draw() {
  graphics->setColor(255, 255, 255);
  graphics->line(0, 0, 1920, 1080);
  graphics->draw(background, 0, 0, 1200, 720);
}

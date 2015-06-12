#include "play.h"

Play::Play() {
  base = new Base();

  terminals[0] = new Terminal();
}

Play::~Play() {
  delete base;

  for (int i = 0; i < TERMINAL_COUNT; i++)
    delete terminals[i];
}

void Play::update() {
  base->update();
}

void Play::draw() {
  base->draw();
}

//=============================================================================

void Play::leftClick(Position pos) {
  printf("%f %f\n", pos.x, pos.y);
  base->moveTo(pos);
}

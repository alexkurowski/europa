#include "play.h"

Play::Play() {
  base   = new Base();
  screen = new Screen();

  Terminal* terminal = new Terminal();
  base->addTerminal(terminal);
  screen->addTerminal(terminal);
}

Play::~Play() {
  delete base;
  delete screen;
}

void Play::update() {
  base->update();

  screen->setNextTerminal(base->currentTerminal());
  screen->update();
}

void Play::draw() {
  Graphics::I()->baseScale();
  base->draw();
  Graphics::I()->screenScale();
  screen->draw();
}

//=============================================================================

void Play::leftClick(Position pos) {
  base->moveTo(pos);
}

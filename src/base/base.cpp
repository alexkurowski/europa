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
  Graphics::I()->draw(background.texture, &background.rect);
  for (int i = 0; i < terminalCount; i++) {
    Graphics::I()->draw(terminals[i]->texture, &terminals[i]->rect);
  }

  character->draw();
}

//=============================================================================

void Base::addTerminal(Terminal* term) {
  terminals[terminalCount] = new Texture();
  terminals[terminalCount]->texture = Graphics::I()->loadTexture("assets/images/base_terminal.png");
  Graphics::I()->setSize(terminals[terminalCount]->texture, terminals[terminalCount]->rect);
  terminals[terminalCount]->rect.x = 320;
  terminals[terminalCount]->rect.y = 600;
  terminalCount++;
}

int Base::currentTerminal() {
  if (character->isStanding())
    return _currentTerminal;
  else
    return -1;
}

//=============================================================================

void Base::moveTo(Position pos) {
  if (pos.x < background.rect.x ||
      pos.x > background.rect.x + background.rect.w ||
      pos.y < background.rect.y ||
      pos.y > background.rect.y + background.rect.h)
    return;

  alignToTerminal(&pos);
  character->moveTo(pos);
}

void Base::alignToTerminal(Position* pos) {
  for (int i = 0; i < terminalCount; i++) {
    if (pos->x > terminals[i]->rect.x &&
        pos->x < terminals[i]->rect.x + terminals[i]->rect.w &&
        pos->y > terminals[i]->rect.y &&
        pos->y < terminals[i]->rect.y + terminals[i]->rect.h) {

      pos->x = terminals[i]->rect.x + terminals[i]->rect.w / 2;
      pos->y = terminals[i]->rect.y + terminals[i]->rect.h / 2;

      _currentTerminal = i;

      return;
    }
  }

  _currentTerminal = -1;
}

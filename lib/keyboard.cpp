#include "keyboard.h"
#include <stdio.h>

Keyboard* Keyboard::_instance = NULL;

Keyboard::Keyboard() {}
Keyboard::~Keyboard() {}

Keyboard* Keyboard::I() {
  if (!_instance)
    _instance = new Keyboard();

  return _instance;
}

void Keyboard::setup() {
  for (int i = 0; i < INPUT_LENGTH; i++) {
    keys[i] = -1;
  }
}

void Keyboard::update() {
  for (int i = 0; i < INPUT_LENGTH; i++) {
    shellKeys[i] = isDownShell(i);

    if (keys[i] >= 0 && keys[i] < ignoreEnd)
      keys[i] += Graphics::I()->dt();
  }
}

void Keyboard::after() {
}

//=============================================================================

void Keyboard::keyDown(SDL_KeyboardEvent& event) {
  if (event.keysym.sym < INPUT_LENGTH) {
    keys[event.keysym.sym] = 0;
  }

  if (event.keysym.sym == SDLK_RSHIFT || event.keysym.sym == SDLK_LSHIFT)
    modKeys[0] = true;
  if (event.keysym.sym == SDLK_RCTRL || event.keysym.sym == SDLK_LCTRL)
    modKeys[1] = true;
  if (event.keysym.sym == SDLK_RALT || event.keysym.sym == SDLK_LALT)
    modKeys[2] = true;
}

void Keyboard::keyUp(SDL_KeyboardEvent& event) {
  if (event.keysym.sym < INPUT_LENGTH) {
    keys[event.keysym.sym] = -1;
  }

  if (event.keysym.sym == SDLK_RSHIFT || event.keysym.sym == SDLK_LSHIFT)
    modKeys[0] = false;
  if (event.keysym.sym == SDLK_RCTRL || event.keysym.sym == SDLK_LCTRL)
    modKeys[1] = false;
  if (event.keysym.sym == SDLK_RALT || event.keysym.sym == SDLK_LALT)
    modKeys[2] = false;
}

//=============================================================================

bool Keyboard::isDownShell(char i) {
  if (keys[i] >= 0 && !(keys[i] > ignoreStart && keys[i] < ignoreEnd))
    return true;
  else
    return false;
}

bool* Keyboard::getShellKeys() {
  return shellKeys;
}

bool* Keyboard::getModKeys() {
  return modKeys;
}

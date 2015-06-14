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
  for (int i = 0; i < INPUT_LENGTH; i++)
    keys[i] = -1;

  for (int i = 0; i < 4; i++)
    arrowKeys[i] = -1;
}

void Keyboard::update() {
  updateShellKeys();

  for (int i = 0; i < INPUT_LENGTH; i++)
    if (keys[i] >= 0 && keys[i] < ignoreEnd)
      keys[i] += Graphics::I()->dt();

  for (int i = 0; i < 4; i++)
    if (arrowKeys[i] >= 0 && arrowKeys[i] < ignoreEnd)
      arrowKeys[i] += Graphics::I()->dt();
}

void Keyboard::after() {
}

//=============================================================================

void Keyboard::keyEvent(SDL_KeyboardEvent& event) {
  bool down = (event.type == SDL_KEYDOWN);
  int idown = (down ? 0 : -1);

  if (event.keysym.sym < INPUT_LENGTH) {
    keys[event.keysym.sym] = idown;
  }

  if (event.keysym.sym == SDLK_RSHIFT || event.keysym.sym == SDLK_LSHIFT)
    modKeys[0] = down;
  if (event.keysym.sym == SDLK_RCTRL || event.keysym.sym == SDLK_LCTRL)
    modKeys[1] = down;
  if (event.keysym.sym == SDLK_RALT || event.keysym.sym == SDLK_LALT)
    modKeys[2] = down;
  if (event.keysym.sym == SDLK_DELETE)
    modKeys[3] = down;

  if (event.keysym.sym == SDLK_LEFT)
    arrowKeys[0] = idown;
  if (event.keysym.sym == SDLK_RIGHT)
    arrowKeys[1] = idown;
  if (event.keysym.sym == SDLK_UP)
    arrowKeys[2] = idown;
  if (event.keysym.sym == SDLK_DOWN)
    arrowKeys[3] = idown;
}

//=============================================================================

bool Keyboard::isDownShell(float value) {
  if (value >= 0 && !(value > ignoreStart && value < ignoreEnd))
    return true;
  else
    return false;
}

shellInputKeys* Keyboard::getShellKeys() {
  return &shellKeys;
}

bool* Keyboard::getModKeys() {
  return modKeys;
}

float* Keyboard::getArrowKeys() {
  return arrowKeys;
}

//=============================================================================

void Keyboard::updateShellKeys() {
  for (int i = 32; i <= 126; i++)
    shellKeys.text[i] = isDownShell(keys[i]);

  shellKeys.Backspace = isDownShell(keys[8]);
  shellKeys.Delete    = isDownShell(keys[127]);
  shellKeys.Enter     = isDownShell(keys[13]);

  shellKeys.Left  = isDownShell(arrowKeys[0]);
  shellKeys.Right = isDownShell(arrowKeys[1]);
  shellKeys.Up    = isDownShell(arrowKeys[2]);
  shellKeys.Down  = isDownShell(arrowKeys[3]);
}

int Keyboard::getShellInputKey(int i) {
  if (i == 32 || i >= 48 && i <= 57 && !modKeys[0]) {
    return i;
  } else
  if (i >= 97 && i <= 122) {
    return i - 32;
  } else
  if (i == 91 || i == 93 || i == 92) {
    if (modKeys[0]) return i + 32;
    else            return i;
  } else
  if (i == 44 || i == 46 || i == 47) {
    if (modKeys[0]) return i + 16;
    else            return i;
  } else
  if (i == 39) {
    if (modKeys[0]) return i - 5;
    else            return i;
  } else
  if (i == 59) {
    if (modKeys[0]) return i - 1;
    else            return i;
  } else
  if (i == 45) {
    if (modKeys[0]) return i + 50;
    else            return i;
  } else
  if (i == 61) {
    if (modKeys[0]) return i - 18;
    else            return i;
  } else
  if (i == 96) {
    if (modKeys[0]) return i + 30;
    else            return i;
  } else
  if (i == 48) {
    return 41;
  } else
  if (i == 49) {
    return 33;
  } else
  if (i == 50) {
    return 64;
  } else
  if (i >= 51 && i <= 53) {
    return i - 16;
  } else
  if (i == 54) {
    return 94;
  } else
  if (i == 55) {
    return 38;
  } else
  if (i == 56) {
    return 42;
  } else
  if (i == 57) {
    return 40;
  }
}

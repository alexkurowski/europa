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
    shellKeys[i]  = false;
    bitmapKeys[i] = false;
  }

  for (int i = 0; i < 4; i++)
    arrowKeys[i] = -1;
}

void Keyboard::update() {
  updateShellKeys();
  updateBitmapKeys();

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
  if (event.keysym.sym == SDLK_ESCAPE)
    modKeys[4] = down;

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
  return value >= 0 && !(value > ignoreStart && value < ignoreEnd);
}

bool Keyboard::isDownBitmap(float value) {
  return value >= 0;
}

bool* Keyboard::getKeys(bool shellMode) {
  if (shellMode)
    return getShellKeys();
  else
    return getBitmapKeys();
}

bool* Keyboard::getShellKeys() {
  return shellKeys;
}

bool* Keyboard::getBitmapKeys() {
  return bitmapKeys;
}

bool* Keyboard::getModKeys() {
  return modKeys;
}

float* Keyboard::getArrowKeys() {
  return arrowKeys;
}

//=============================================================================

void Keyboard::updateShellKeys() {
  for (int i = CHAR_TEXT_START; i <= CHAR_TEXT_END; i++)
    shellKeys[getTextInputKey(i)] = isDownShell(keys[i]);

  shellKeys[CHAR_ARROW_LEFT]  = isDownShell(arrowKeys[0]);
  shellKeys[CHAR_ARROW_RIGHT] = isDownShell(arrowKeys[1]);
  shellKeys[CHAR_ARROW_UP]    = isDownShell(arrowKeys[2]);
  shellKeys[CHAR_ARROW_DOWN]  = isDownShell(arrowKeys[3]);

  shellKeys[CHAR_BACKSPACE] = isDownShell(keys[8]);
  shellKeys[CHAR_DELETE]    = isDownShell(keys[127]);
  shellKeys[CHAR_ENTER]     = isDownShell(keys[13]);

  shellKeys[CHAR_SHIFT]  = modKeys[0];
  shellKeys[CHAR_CTRL]   = modKeys[1];
  shellKeys[CHAR_ALT]    = modKeys[2];
  shellKeys[CHAR_ESCAPE] = modKeys[3];
}

void Keyboard::updateBitmapKeys() {
  for (int i = CHAR_TEXT_START; i <= CHAR_TEXT_END; i++)
    bitmapKeys[getTextInputKey(i)] = isDownBitmap(keys[i]);

  bitmapKeys[CHAR_ARROW_LEFT]  = isDownBitmap(arrowKeys[0]);
  bitmapKeys[CHAR_ARROW_RIGHT] = isDownBitmap(arrowKeys[1]);
  bitmapKeys[CHAR_ARROW_UP]    = isDownBitmap(arrowKeys[2]);
  bitmapKeys[CHAR_ARROW_DOWN]  = isDownBitmap(arrowKeys[3]);

  bitmapKeys[CHAR_BACKSPACE] = isDownBitmap(keys[8]);
  bitmapKeys[CHAR_DELETE]    = isDownBitmap(keys[127]);
  bitmapKeys[CHAR_ENTER]     = isDownBitmap(keys[13]);

  bitmapKeys[CHAR_SHIFT]  = modKeys[0];
  bitmapKeys[CHAR_CTRL]   = modKeys[1];
  bitmapKeys[CHAR_ALT]    = modKeys[2];
  bitmapKeys[CHAR_ESCAPE] = modKeys[3];
}

int Keyboard::getTextInputKey(int i) {
  // ignore shift modified letters
  if (i == 64  || i == 126 || i == 63 ||
      i == 60  || i == 62  || i == 94 ||
      i == 123 || i == 125 || i == 95 ||
      i == 124) return 0;

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
    return i - 7;
  } else
  if (i == 50) {
    return i + 14;
  } else
  if (i == 49 || i >= 51 && i <= 53) {
    return i - 16;
  } else
  if (i == 54) {
    return i + 40;
  } else
  if (i == 55 || i == 57) {
    return i - 17;
  } else
  if (i == 56) {
    return i - 14;
  }
  return i;
}

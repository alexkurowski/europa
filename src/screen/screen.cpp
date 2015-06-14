#include "screen.h"
#include <stdio.h>

Screen::Screen() {
  fadeRect.x = 0;
  fadeRect.y = 0;
  fadeRect.w = Graphics::I()->originalWidth();
  fadeRect.h = Graphics::I()->originalHeight();
}

Screen::~Screen() {}

void Screen::update() {
  for (int i = 0; i < termCount; i++) {
    terminals[i]->update();
  }

  if (currentTerminal == -1 && nextTerminal == -1) return;

  if (nextTerminal != currentTerminal || fade < 255) {
    updateFade();
  }
}

void Screen::draw() {
  if (fade > 0) {
    Graphics::I()->setColor(0, 0, 0, fade * 0.8);
    Graphics::I()->setBlendMode("blend");
    Graphics::I()->rectangle(&fadeRect);

    terminal()->draw(fade);
  }
}

//=============================================================================

void Screen::addTerminal(Terminal* term) {
  terminals[termCount] = term;
  termCount++;
}

void Screen::setNextTerminal(int id) {
  nextTerminal = id;
}

bool Screen::isAtTerminal() {
  return currentTerminal != -1;
}

void Screen::setCurrentTerminal() {
  currentTerminal = nextTerminal;
  if (isAtTerminal() && !terminal()->isActive())
    terminal()->turnOn();
}

Terminal* Screen::terminal() {
  if (isAtTerminal())
    return terminals[currentTerminal];
  return NULL;
}

//=============================================================================

void Screen::updateFade() {
  if (currentTerminal != nextTerminal)
    fadeOut();
  else if (currentTerminal == -1)
    fadeAway();
  else
    fadeIn();
}

void Screen::fadeOut() {
  fade -= fadeSpeed * Graphics::I()->dt();
  if (fade < 0) {
    fade = 0;
    setCurrentTerminal();
  }
}

void Screen::fadeIn() {
  if (fade < 255) {
    fade += fadeSpeed * Graphics::I()->dt();
    if (fade > 255) fade = 255;
  }
}

void Screen::fadeAway() {
  if (fade > 0) {
    fade -= fadeSpeed * Graphics::I()->dt();
    if (fade < 0) fade = 0;
  }
}

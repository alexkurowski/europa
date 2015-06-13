#include "game.h"

Game::Game() {
  state = new Play();
}

Game::~Game() {
  delete state;
}

void Game::input() {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEBUTTONDOWN:
        Position pos;
        pos.x = event.button.x * Graphics::I()->originalScale();
        pos.y = event.button.y * Graphics::I()->originalScale();
        if (event.button.button == SDL_BUTTON_LEFT)
          state->leftClick(pos);
        if (event.button.button == SDL_BUTTON_RIGHT)
          state->rightClick(pos);
        break;

      case SDL_QUIT:
        active = false;
        break;
    }
  }
}

void Game::update() {
  state->update();

  // printFPS();
}

void Game::draw() {
  state->draw();
}


void Game::printFPS() {
  frame++;
  int currentTime = SDL_GetTicks();
  if (currentTime - sec > 1000) {
    sec = currentTime;
    printf("%i\n", frame);
    frame = 0;
  }
}

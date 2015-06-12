#include "game.h"

Game::Game() {
  quit = false;

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
        quit = true;
        break;
    }
  }
}

void Game::update() {
  state->update();
}

void Game::draw() {
  state->draw();
}

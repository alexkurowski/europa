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

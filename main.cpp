#include <iostream>

#include <SDL2/SDL.h>
#include <iostream>
#include "lib/sdlgraphics.h"
#include "src/game.h"

const int ORIGINAL_WIDTH  = 1920;
const int ORIGINAL_HEIGHT = 1080;
const char* WINDOW_TITLE = "Europa";


SDLGraphics* graphics = NULL;

Game* game = NULL;

int main() {
  // Initializations
  graphics = new SDLGraphics(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, WINDOW_TITLE);

  game = new Game();

  // main loop
  // Game game;
  while (!game->quit) {
    game->input();

    game->update();

    graphics->beginDraw();
    game->draw();
    graphics->endDraw();
  }

  delete game;
  delete graphics;
  return 0;
}


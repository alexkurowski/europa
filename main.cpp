#include <iostream>

#include <SDL2/SDL.h>
#include <iostream>
#include "lib/graphics.h"
#include "src/game.h"

const int ORIGINAL_WIDTH  = 1920;
const int ORIGINAL_HEIGHT = 1080;
const char* WINDOW_TITLE = "Europa";


Game* game = NULL;


int main() {
  // Initializations
  Graphics::I()->setup(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, WINDOW_TITLE);

  game = new Game();

  // main loop
  // Game game;
  while (!game->quit) {
    game->input();

    game->update();

    Graphics::I()->beginDraw();
    game->draw();
    Graphics::I()->endDraw();
  }

  delete game;
  return 0;
}


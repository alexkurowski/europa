#include <iostream>

#include <SDL2/SDL.h>
#include <iostream>
#include "lib/graphics.h"
#include "lib/keyboard.h"
#include "src/game.h"

const int ORIGINAL_WIDTH  = 1920;
const int ORIGINAL_HEIGHT = 1080;
const char* WINDOW_TITLE = "Europa";


Game* game = NULL;


int main() {
  // Initializations
  Graphics::I()->setup(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, WINDOW_TITLE);
  Keyboard::I()->setup();

  game = new Game();

  // main loop
  // Game game;
  while (game->active) {
    if (Graphics::I()->cap()) continue;

    game->input();

    Keyboard::I()->update();
    game->update();
    Keyboard::I()->after();

    Graphics::I()->beginDraw();
    game->draw();
    Graphics::I()->endDraw();
  }

  delete game;
  return 0;
}


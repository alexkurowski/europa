#pragma once

#include <stdio.h>
#include <stdint.h>
#include "../../lib/graphics.h"
#include "../../lib/data.h"

class Terminal {
  public:
    Terminal();
    ~Terminal();

    void update();
    void draw(int);

  private:
    int p;
    uint8_t memory[TERMINAL_MEMORY];

    Position offset;
    const int margin = 40;

    void drawBackground(int);
    void drawScreen(int);

    int randomRange(int, int);

    const uint8_t colors[16][3] = {
      {20,  20,  20},  // 0 - black
      {240, 240, 240}, // 1 - white
      {150, 20,  20},  // 2 - red
      {130, 240, 220}, // 3 - cyan
      {240, 70,  240}, // 4 - violet
      {20,  240, 100}, // 5 - green
      {50,  50,  220}, // 6 - default (default bg)
      {250, 250, 100}, // 7 - yellow
      {240, 100, 60},  // 8 - orange
      {150, 60,  20},  // 9 - brown
      {250, 130, 140}, // 10 - lightred
      {70,  70,  70},  // 11 - grey 1
      {140, 140, 140}, // 12 - grey 2
      {170, 250, 90},  // 13 - lightgreen
      {20,  150, 250}, // 14 - lightblue (default fg)
      {210, 210, 210}  // 15 - grey 3
    };

    const uint8_t colorDefaultBG = 6;
    const uint8_t colorDefaultFG = 14;
};

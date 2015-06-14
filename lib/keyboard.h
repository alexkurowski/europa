#pragma once

#include <SDL2/SDL.h>
#include "graphics.h"

#define INPUT_LENGTH 0x80


class Keyboard {
  public:
    static Keyboard* I();

    void setup();

    void update();
    void after();

    void keyDown(SDL_KeyboardEvent&);
    void keyUp(SDL_KeyboardEvent&);

    bool isUp(char);
    bool isDown(char);

    bool* getShellKeys();
    bool* getModKeys();

  private:
    Keyboard();
    ~Keyboard();
    Keyboard(Keyboard const&){};
    Keyboard& operator=(Keyboard const&){};

    static Keyboard* _instance;

    float keys[INPUT_LENGTH];
    bool shellKeys[INPUT_LENGTH] = { false };
    bool modKeys[3] = { false }; // shift, ctrl, alt

    bool isDownShell(char);

    uint8_t ignoreStart = 0;
    uint8_t ignoreEnd = 1;
};

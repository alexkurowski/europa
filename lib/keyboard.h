#pragma once

#include <SDL2/SDL.h>
#include "graphics.h"

#define INPUT_LENGTH 0x80

struct shellInputKeys {
  bool text[INPUT_LENGTH] = { false };

  bool Delete;
  bool Backspace;
  bool Enter;

  bool Left;
  bool Right;
  bool Up;
  bool Down;
};

class Keyboard {
  public:
    static Keyboard* I();

    void setup();

    void update();
    void after();

    void keyEvent(SDL_KeyboardEvent&);

    bool isUp(char);
    bool isDown(char);

    shellInputKeys* getShellKeys();
    bool* getModKeys();
    float* getArrowKeys();

    int getShellInputKey(int);

  private:
    Keyboard();
    ~Keyboard();
    Keyboard(Keyboard const&){};
    Keyboard& operator=(Keyboard const&){};

    static Keyboard* _instance;

    float keys[INPUT_LENGTH];
    shellInputKeys shellKeys;
    bool modKeys[4] = { false }; // shift, ctrl, alt, delete
    float arrowKeys[4] = { 0 }; // left, right, up, down

    bool isDownShell(float);
    void updateShellKeys();

    uint8_t ignoreStart = 0;
    uint8_t ignoreEnd = 1;
};

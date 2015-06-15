#pragma once

#include <SDL2/SDL.h>
#include "graphics.h"


#define INPUT_LENGTH       0x80

#define CHAR_ARROW_UP      1
#define CHAR_ARROW_LEFT    2
#define CHAR_ARROW_DOWN    3
#define CHAR_ARROW_RIGHT   4

#define CHAR_BACKSPACE     8
#define CHAR_DELETE        9
#define CHAR_ENTER         13

#define CHAR_SHIFT         16
#define CHAR_CTRL          17
#define CHAR_ALT           18

#define CHAR_ESCAPE        27

#define CHAR_TEXT_START    32
#define CHAR_TEXT_END      126


class Keyboard {
  public:
    static Keyboard* I();

    void setup();

    void update();
    void after();

    void keyEvent(SDL_KeyboardEvent&);

    bool isUp(char);
    bool isDown(char);

    bool*  getKeys(bool);
    bool*  getShellKeys();
    bool*  getBitmapKeys();
    bool*  getModKeys();
    float* getArrowKeys();


  private:
    Keyboard();
    ~Keyboard();
    Keyboard(Keyboard const&){};
    Keyboard& operator=(Keyboard const&){};

    static Keyboard* _instance;

    float keys[INPUT_LENGTH];
    bool  shellKeys[INPUT_LENGTH];
    bool  bitmapKeys[INPUT_LENGTH];
    bool  modKeys[5] = { false }; // shift, ctrl, alt, delete, escape
    float arrowKeys[4] = { 0 }; // left, right, up, down

    bool isDownShell(float);
    bool isDownBitmap(float);

    void updateShellKeys();
    void updateBitmapKeys();

    int  getTextInputKey(int);

    uint8_t ignoreStart = 0;
    uint8_t ignoreEnd = 1;
};

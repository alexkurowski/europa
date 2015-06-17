#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../lib/graphics.h"
#include "../../lib/keyboard.h"
#include "../../lib/data.h"

#include "components/memory.h"
#include "components/display.h"
#include "components/shell.h"
#include "components/program.h"

// #define TERMINAL_MEMORY     0xFFFF
// #define TERMINAL_STACK_SIZE 0x0F
//
// #define SHELL_WIDTH         0x28
// #define SHELL_HEIGHT        0x19
//
// #define CURRENT_ADDRESS     0x0010
// #define PREVIOUS_ADDRESS    0x0012
//
// #define KEYBOARD_INPUT      0x0020
//
// #define SCREEN_MODE         0x0040
//
// #define FONT_ADDRESS        0x0100
//
// #define USER_ADDRESS        0x1000
// #define END_USER_ADDRESS    0x7FFF
//
// #define ACTIVE_INPUT        0x8400
// #define INPUT_ROW           0x8401
// #define INPUT_COL           0x8402
// #define BLINK_COUNTER       0x8403
//
// #define SCREEN_ADDRESS      0x8000
// #define SHELL_SCREEN_END    0x83E8
// #define BITMAP_SCREEN_END   0xFCFF
// #define CURRENT_COLORS      0xFD00
// #define COLORS_ADDRESS      0xFD01


class Terminal {
  public:
    Terminal();
    ~Terminal();

    void update();
    void draw(uint8_t);

    void turnOn();
    void turnOff();
    bool isActive();

  private:
    Memory*  mem;
    Display* display;
    Shell*   shell;
    Program* prog;

    bool active = false;

    void setSleep(float, void (Terminal::*)());
    void setSleep(float);
    float sleep = 0;
    void (Terminal::*afterSleep)() = NULL;
    bool doAfterSleep;
    bool isSleeping();

    const float bootTime   = 0.6; // 1.2
    const float inputDelay = 0.6;

    void afterBoot();
    void beforeInput();
    void readyInput();

    void updateProgram();
    void updateShell();

    void shellMessage(std::string);


    // void resetShell();
    // void readyBeforeInput();
    // void readyInput();
    // void processInput();

    // void processCommand();
    // void shellPrintOutput();


    // bool inShell();
    // bool inBitmap();

    // void input();
    // void shellInput();
    // uint16_t inputGetCurrentPosition();
    // uint16_t inputGetStartOfLine();
    // uint16_t inputGetEndOfLine();
    // uint8_t inputGetCharacter();
    // void inputPutCharacter(uint8_t);

    // void shellNextLine();
    // void shellCommandRun();
    // void shellCommandEdit();
    // void shellCommandList();
    // void shellCommandHelp();

    // void loadColors();

    // void drawBackground(int);
    // void drawScreen(int);

    // void drawShellMode(int);
    // void drawBitmapMode(int);

    // uint8_t randomRange(int, int);

    std::string command;
    std::string output;

    uint8_t colors[16][3];
    uint8_t fg;
    uint8_t bg;

    uint8_t originalColor = 0x6e;
    const uint8_t originalColors[16][3] = {
      { 0x16, 0x16, 0x16 }, // 0 - black
      { 0xf0, 0xf0, 0xf0 }, // 1 - white
      { 0xa0, 0x16, 0x16 }, // 2 - red
      { 0x80, 0xf0, 0xdd }, // 3 - cyan
      { 0xf0, 0x46, 0xf0 }, // 4 - violet
      { 0x16, 0xf0, 0x64 }, // 5 - green
      { 0x32, 0x32, 0xdd }, // 6 - default (default bg)
      { 0xfa, 0xfa, 0x64 }, // 7 - yellow
      { 0xf0, 0x64, 0x3d }, // 8 - orange
      { 0xa0, 0x3d, 0x16 }, // 9 - brown
      { 0xfa, 0x80, 0x8c }, // a - lightred
      { 0x46, 0x46, 0x46 }, // b - grey 1
      { 0x8c, 0x8c, 0x8c }, // c - grey 2
      { 0xaa, 0xfa, 0x60 }, // d - lightgreen
      { 0x16, 0xa0, 0xfa }, // e - lightblue (default fg)
      { 0xd2, 0xd2, 0xd2 }  // f - grey 3
    };

    const uint8_t colorDefaultBG = 6;
    const uint8_t colorDefaultFG = 14;

    const uint8_t originalFont[256 * 8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x81,0xa5,0x81,0xbd,0x99,0x42,0x3c,0x7e,0xff,0xdb,0xff,0xc3,0xe7,0x7e,0x3c,0x36,0x7f,0x7f,0x7f,0x3e,0x1c,0x08,0x00,0x08,0x1c,0x3e,0x7f,0x3e,0x1c,0x08,0x00,0x1c,0x3e,0x1c,0x7f,0x7f,0x6b,0x08,0x1c,0x08,0x08,0x1c,0x3e,0x7f,0x3e,0x08,0x1c,0x00,0x00,0x18,0x3c,0x3c,0x18,0x00,0x00,0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff,0x00,0x3c,0x66,0x42,0x42,0x66,0x3c,0x00,0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff,0x0f,0x07,0x0f,0x7d,0xcc,0xcc,0xcc,0x78,0x3c,0x66,0x66,0x66,0x3c,0x18,0x7e,0x18,0x08,0x0c,0x0e,0x0a,0x0a,0x38,0x78,0x30,0x00,0x3e,0x22,0x22,0x26,0x6e,0xe4,0x40,0x18,0xdb,0x3c,0xe7,0xe7,0x3c,0xdb,0x18,0x20,0x30,0x38,0x3c,0x38,0x30,0x20,0x00,0x04,0x0c,0x1c,0x3c,0x1c,0x0c,0x04,0x00,0x18,0x3c,0x7e,0x18,0x18,0x7e,0x3c,0x18,0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x00,0x7f,0xdb,0xdb,0x7b,0x1b,0x1b,0x1b,0x00,0x3c,0x60,0x3c,0x66,0x3c,0x06,0x3c,0x00,0x00,0x00,0x00,0x00,0x7e,0x7e,0x00,0x00,0x18,0x3c,0x7e,0x18,0x7e,0x3c,0x18,0xff,0x18,0x3c,0x7e,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,0x00,0x0c,0x06,0x7f,0x06,0x0c,0x00,0x00,0x00,0x18,0x30,0x7f,0x30,0x18,0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x7e,0x00,0x00,0x00,0x24,0x66,0xff,0x66,0x24,0x00,0x00,0x00,0x08,0x1c,0x3e,0x7f,0x00,0x00,0x00,0x00,0x00,0x7f,0x3e,0x1c,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3c,0x3c,0x18,0x18,0x00,0x18,0x00,0x36,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x36,0x36,0x7f,0x36,0x7f,0x36,0x36,0x00,0x18,0x3e,0x60,0x3c,0x06,0x7c,0x18,0x00,0x60,0x66,0x0c,0x18,0x30,0x66,0x06,0x00,0x38,0x66,0x66,0x3f,0x66,0x66,0x3b,0x00,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x0c,0x18,0x30,0x30,0x30,0x18,0x0c,0x00,0x30,0x18,0x0c,0x0c,0x0c,0x18,0x30,0x00,0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x06,0x0c,0x18,0x30,0x60,0x00,0x00,0x3c,0x66,0x6e,0x7e,0x76,0x66,0x3c,0x00,0x18,0x38,0x18,0x18,0x18,0x18,0x7e,0x00,0x3c,0x66,0x06,0x1c,0x30,0x66,0x7e,0x00,0x3c,0x66,0x06,0x1c,0x06,0x66,0x3c,0x00,0x0e,0x1e,0x36,0x66,0x7f,0x06,0x0f,0x00,0x7e,0x60,0x60,0x7c,0x06,0x66,0x3c,0x00,0x1c,0x30,0x60,0x7c,0x66,0x66,0x3c,0x00,0x7e,0x66,0x06,0x0c,0x18,0x18,0x18,0x00,0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00,0x3c,0x66,0x66,0x3e,0x06,0x0c,0x38,0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x30,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x00,0x00,0x00,0x7e,0x00,0x7e,0x00,0x00,0x00,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x00,0x3c,0x66,0x06,0x0c,0x18,0x00,0x18,0x00,0x3c,0x66,0x6e,0x6a,0x6e,0x60,0x3c,0x00,0x3c,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,0x7c,0x66,0x66,0x7c,0x66,0x66,0x7c,0x00,0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00,0x78,0x6c,0x66,0x66,0x66,0x6c,0x78,0x00,0x7e,0x60,0x60,0x7c,0x60,0x60,0x7e,0x00,0x7e,0x60,0x60,0x7c,0x60,0x60,0x60,0x00,0x3c,0x66,0x60,0x6e,0x66,0x66,0x3c,0x00,0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x0e,0x06,0x06,0x06,0x66,0x66,0x3c,0x00,0x66,0x66,0x6c,0x78,0x6c,0x66,0x66,0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00,0x63,0x77,0x7f,0x7f,0x6b,0x63,0x63,0x00,0x66,0x76,0x7e,0x7e,0x6e,0x66,0x66,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00,0x3c,0x66,0x66,0x66,0x6e,0x3c,0x0e,0x00,0x7c,0x66,0x66,0x7c,0x6c,0x66,0x66,0x00,0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x63,0x63,0x63,0x6b,0x7f,0x77,0x63,0x00,0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00,0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x00,0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00,0x3e,0x30,0x30,0x30,0x30,0x30,0x3e,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x3e,0x06,0x06,0x06,0x06,0x06,0x3e,0x00,0x08,0x1c,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x38,0x18,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x60,0x60,0x7c,0x66,0x66,0x66,0x7c,0x00,0x00,0x00,0x3c,0x66,0x60,0x66,0x3c,0x00,0x06,0x06,0x3e,0x66,0x66,0x66,0x3e,0x00,0x00,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00,0x1e,0x30,0x30,0x7c,0x30,0x30,0x30,0x00,0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x3c,0x60,0x60,0x7c,0x66,0x66,0x66,0x66,0x00,0x18,0x00,0x38,0x18,0x18,0x18,0x3c,0x00,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x30,0x60,0x60,0x66,0x6c,0x78,0x6c,0x66,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x0c,0x00,0x00,0x00,0x36,0x7f,0x6b,0x6b,0x63,0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x07,0x00,0x00,0x7c,0x66,0x60,0x60,0x60,0x00,0x00,0x00,0x3e,0x60,0x3c,0x06,0x7c,0x00,0x30,0x30,0x7c,0x30,0x30,0x30,0x1c,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3e,0x00,0x00,0x00,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x63,0x6b,0x6b,0x7f,0x36,0x00,0x00,0x00,0x66,0x3c,0x18,0x3c,0x66,0x00,0x00,0x00,0x66,0x66,0x66,0x3e,0x06,0x3c,0x00,0x00,0x7e,0x0c,0x18,0x30,0x7e,0x00,0x1c,0x30,0x30,0x60,0x30,0x30,0x1c,0x00,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0x38,0x0c,0x0c,0x06,0x0c,0x0c,0x38,0x00,0x00,0x00,0x3b,0x6e,0x00,0x00,0x00,0x00,0x00,0x08,0x1c,0x36,0x63,0x63,0x7f,0x00,0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x60,0x66,0x00,0x66,0x66,0x66,0x66,0x3e,0x00,0x0c,0x18,0x3c,0x66,0x7e,0x60,0x3c,0x00,0x7e,0xc3,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x66,0x00,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x30,0x18,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x18,0x24,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x00,0x00,0x3c,0x66,0x60,0x66,0x3c,0x60,0x7e,0xc3,0x3c,0x66,0x7e,0x60,0x3c,0x00,0x66,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00,0x30,0x18,0x3c,0x66,0x7e,0x60,0x3c,0x00,0x66,0x00,0x38,0x18,0x18,0x18,0x3c,0x00,0x3c,0x66,0x00,0x38,0x18,0x18,0x3c,0x00,0x30,0x18,0x00,0x38,0x18,0x18,0x3c,0x00,0x66,0x00,0x3c,0x66,0x7e,0x66,0x66,0x00,0x18,0x24,0x3c,0x66,0x7e,0x66,0x66,0x00,0x0c,0x18,0x7e,0x60,0x7c,0x60,0x7e,0x00,0x00,0x00,0x7e,0x1b,0x7f,0xd8,0x7e,0x00,0x1f,0x3c,0x6c,0x7f,0x6c,0x6c,0x6f,0x00,0x3c,0x66,0x00,0x3c,0x66,0x66,0x3c,0x00,0x00,0x66,0x00,0x3c,0x66,0x66,0x3c,0x00,0x30,0x18,0x00,0x3c,0x66,0x66,0x3c,0x00,0x3c,0x66,0x00,0x66,0x66,0x66,0x3e,0x00,0x30,0x18,0x00,0x66,0x66,0x66,0x3e,0x00,0x66,0x00,0x66,0x66,0x66,0x3e,0x06,0x3c,0x66,0x3c,0x66,0x66,0x66,0x66,0x3c,0x00,0x66,0x00,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x0c,0x1e,0x30,0x30,0x1e,0x0c,0x00,0x1c,0x36,0x30,0x7c,0x30,0x60,0x7e,0x00,0x66,0x66,0x3c,0x7e,0x18,0x7e,0x18,0x00,0x7c,0x66,0x6e,0x7c,0x6e,0x6c,0x66,0x00,0x0e,0x18,0x18,0x3c,0x18,0x18,0x70,0x00,0x0c,0x18,0x3c,0x06,0x3e,0x66,0x3e,0x00,0x0c,0x18,0x00,0x38,0x18,0x18,0x3c,0x00,0x0c,0x18,0x00,0x3c,0x66,0x66,0x3c,0x00,0x0c,0x18,0x00,0x66,0x66,0x66,0x3e,0x00,0x36,0x6c,0x00,0x7c,0x66,0x66,0x66,0x00,0x36,0x6c,0x00,0x66,0x76,0x6e,0x66,0x00,0x38,0x0c,0x3c,0x6c,0x3c,0x00,0x00,0x00,0x38,0x6c,0x6c,0x6c,0x38,0x00,0x00,0x00,0x18,0x00,0x18,0x30,0x60,0x66,0x3c,0x00,0x00,0x00,0x00,0x7e,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x7e,0x06,0x06,0x00,0x00,0x40,0x42,0x44,0x48,0x16,0x23,0x46,0x07,0x40,0x42,0x44,0x48,0x15,0x25,0x47,0x01,0x18,0x00,0x18,0x18,0x3c,0x3c,0x18,0x00,0x00,0x36,0x6c,0xd8,0x6c,0x36,0x00,0x00,0x00,0x6c,0x36,0x1b,0x36,0x6c,0x00,0x00,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0xf8,0x18,0x18,0x18,0x18,0xf8,0xf8,0x18,0x18,0xf8,0xf8,0x18,0x66,0x66,0x66,0xe6,0xe6,0x66,0x66,0x66,0x00,0x00,0x00,0xfe,0xfe,0x66,0x66,0x66,0x00,0xf8,0xf8,0x18,0x18,0xf8,0xf8,0x18,0x66,0xe6,0xe6,0x06,0x06,0xe6,0xe6,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0xfe,0xfe,0x06,0x06,0xe6,0xe6,0x66,0x66,0xe6,0xe6,0x06,0x06,0xfe,0xfe,0x00,0x66,0x66,0x66,0xfe,0xfe,0x00,0x00,0x00,0x10,0xf8,0xf8,0x18,0x18,0xf8,0xf8,0x00,0x00,0x00,0x00,0xf8,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x1f,0x00,0x00,0x00,0x18,0x18,0x18,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x18,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x18,0x18,0x18,0xff,0xff,0x18,0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x1f,0x1f,0x18,0x66,0x66,0x66,0x67,0x67,0x66,0x66,0x66,0x66,0x67,0x67,0x60,0x60,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x60,0x60,0x67,0x67,0x66,0x66,0xe7,0xe7,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0xe7,0xe7,0x66,0x66,0x67,0x67,0x60,0x60,0x67,0x67,0x66,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x66,0xe7,0xe7,0x00,0x00,0xe7,0xe7,0x66,0x18,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x66,0x66,0x66,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x18,0x00,0x00,0x00,0xff,0xff,0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x00,0x00,0x00,0x18,0x1f,0x1f,0x18,0x18,0x1f,0x1f,0x00,0x00,0x1f,0x1f,0x18,0x18,0x1f,0x1f,0x18,0x00,0x00,0x00,0x7f,0x7f,0x66,0x66,0x66,0x66,0x66,0x66,0xe7,0xe7,0x66,0x66,0x66,0x18,0xff,0xff,0x00,0x00,0xff,0xff,0x18,0x18,0x18,0x18,0xf8,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x1f,0x18,0x18,0x18,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x3b,0x6e,0x64,0x6e,0x3b,0x00,0x3c,0x66,0x66,0x6c,0x66,0x63,0x6e,0x60,0x7e,0x66,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x7f,0x36,0x36,0x36,0x36,0x36,0x00,0x7e,0x60,0x30,0x18,0x30,0x60,0x7e,0x00,0x00,0x00,0x3e,0x78,0x6c,0x6c,0x38,0x00,0x00,0x00,0x66,0x66,0x6e,0x7b,0x60,0x60,0x00,0x00,0x7e,0x18,0x18,0x18,0x0c,0x00,0x3c,0x18,0x3c,0x66,0x66,0x3c,0x18,0x3c,0x3c,0x66,0x66,0x7e,0x66,0x66,0x3c,0x00,0x3e,0x63,0x63,0x63,0x63,0x36,0x77,0x00,0x3c,0x30,0x18,0x3c,0x66,0x66,0x3c,0x00,0x00,0x00,0x36,0x6f,0x6b,0x7b,0x36,0x00,0x00,0x03,0x3e,0x66,0x7e,0x66,0x7c,0xc0,0x00,0x00,0x3e,0x60,0x3c,0x60,0x3e,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x7e,0x00,0x7e,0x00,0x7e,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x7e,0x00,0x30,0x18,0x0c,0x18,0x30,0x00,0x7e,0x00,0x0c,0x18,0x30,0x18,0x0c,0x00,0x7e,0x00,0x0e,0x1b,0x1b,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xd8,0xd8,0x70,0x00,0x18,0x00,0x7e,0x00,0x18,0x00,0x00,0x00,0x3b,0x6e,0x00,0x3b,0x6e,0x00,0x00,0x1c,0x36,0x36,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x0f,0x0c,0x0c,0xcc,0x6c,0x3c,0x1c,0x0c,0x3c,0x36,0x36,0x36,0x36,0x00,0x00,0x00,0x38,0x0c,0x18,0x30,0x3c,0x00,0x00,0x00,0x00,0x00,0x3c,0x3c,0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    // "                                        "
    // "        **** CPI 6064 SHELL ****        "
    // "      COPYRIGHT 2001 JUPITER GROUP      "
    // "        64K RAM SYSTEM  32K FREE        "
    // "                                        " // 200
    const char* greetText = "                                                **** CPI 6064 SHELL ****              COPYRIGHT 2001 JUPITER GROUP              64K RAM SYSTEM  32K FREE                                                ";
    const char* newInputLine = "READY.                                  ";
    // " LIST      - DISPLAY NAMES OF AVAILABLE "
    // "             PROGRAMS                   "
    // " RUN NAME  - RUN PROGRAM WITH A GIVEN   "
    // "             NAME                       "
    // " EDIT NAME - LOAD PROGRAM WITH A GIVEN  "
    // "             NAME AND OPEN EDITOR       "
    // "                                        " // 280
    const std::string helpText = " LIST      - DISPLAY NAMES OF AVAILABLE              PROGRAMS                    RUN NAME  - RUN PROGRAM WITH A GIVEN                NAME                        EDIT NAME - LOAD PROGRAM WITH A GIVEN               NAME AND OPEN EDITOR                                               ";

};

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cmath>
#include "data.cpp"

class SDLGraphics {
  public:
    SDLGraphics(int, int, const char*);
    ~SDLGraphics();

    void beginDraw();
    void endDraw();

    float getScale();
    float originalScale();

    SDL_Texture* loadTexture(const char*);

    void setColor(int r, int g, int b, int a = 255);
    void line(int, int, int, int);
    void draw(SDL_Texture*, int, int, int, int);
    void blit(SDL_Surface*, int, int, int, int);

  private:
    Size originalSize;
    Size currentSize;
    SDL_Window*   window;
    SDL_Surface*  surface;
    SDL_Renderer* renderer;

    SDL_Rect* rect;

    float scale;

    void setScale();
};

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cmath>
#include "data.h"

class Graphics {
  public:
    static Graphics* I();

    void setup(int, int, const char*);

    float getScale();
    float originalScale();

    int originalWidth();
    int originalHeight();
    int currentWidth();
    int currentHeight();

    void beginDraw();
    void endDraw();

    SDL_Texture* loadTexture(const char*);

    void setSize(Texture*);
    void setSize(SDL_Texture*, SDL_Rect&);

    void setColor(int r, int g, int b, int a = 255);
    void line(int, int, int, int);
    void draw(SDL_Texture*, int, int, int, int);
    void draw(SDL_Texture*, SDL_Rect*);
    void blit(SDL_Surface*, int, int, int, int);

  private:
    Graphics();
    ~Graphics();
    Graphics(Graphics const&){};
    Graphics& operator=(Graphics const&){};

    static Graphics* _instance;

    Size originalSize;
    Size currentSize;
    SDL_Window*   window;
    SDL_Surface*  surface;
    SDL_Renderer* renderer;

    SDL_Rect* rect;

    float scale;

    void setScale();
};
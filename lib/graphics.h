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

    bool cap();
    float dt();

    void beginDraw();
    void endDraw();

    SDL_Texture* loadTexture(const char*);

    void setSize(Texture*);
    void setSize(SDL_Texture*, SDL_Rect&);

    void baseScale();
    void screenScale();

    Position getScreenScaledOffset();

    void setColor(int r, int g, int b, int a = 255);
    void setBlendMode(const char*);
    void point(int, int);
    void line(int, int, int, int);
    void rectangle(int, int, int, int);
    void rectangle(SDL_Rect*);
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
    float termScale;

    void setScale();

    float _dt;
    unsigned int lastTime = 0;
    unsigned int currentTime;
    const float framerateTime = (float)(1000 / 60);
    const float maxdt = framerateTime / 1000;
};

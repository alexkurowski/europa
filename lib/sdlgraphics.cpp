#include "sdlgraphics.h"
#include <stdio.h>

SDLGraphics::SDLGraphics(int width, int height, const char* title) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    throw "SDL_Init Error";

  SDL_DisplayMode desktopMode;
  SDL_GetCurrentDisplayMode(0, &desktopMode);

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   desktopMode.w, desktopMode.h,
                                   SDL_WINDOW_FULLSCREEN_DESKTOP);

  originalSize.w = width;
  originalSize.h = height;

  currentSize = Size();
  currentSize.w = desktopMode.w;
  currentSize.h = desktopMode.h;

  setScale();

  if (window == NULL)
    throw "SDL_CreateWindow Error";

  surface  = SDL_GetWindowSurface(window);
  renderer = SDL_CreateRenderer(window, -1, 0);
  // texture  = SDL_CreateTextureFromSurface(renderer, surface);

  rect = new SDL_Rect();
}

SDLGraphics::~SDLGraphics() {
  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

//=============================================================================

void SDLGraphics::beginDraw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void SDLGraphics::endDraw() {
  SDL_RenderPresent(renderer);
}

//=============================================================================

void SDLGraphics::setScale() {
  scale = fmin(currentSize.w / (double)originalSize.w,
               currentSize.h / (double)originalSize.h);
}

float SDLGraphics::getScale() {
  return scale || 1;
}

float SDLGraphics::originalScale() {
  return 1 / (scale || 1);
}

//=============================================================================

SDL_Texture* SDLGraphics::loadTexture(const char* path) {
  return IMG_LoadTexture(renderer, path);
}

//=============================================================================

void SDLGraphics::setColor(int r, int g, int b, int a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SDLGraphics::line(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1 * scale,
                               y1 * scale,
                               x2 * scale,
                               y2 * scale);
}

void SDLGraphics::draw(SDL_Texture* source, int x, int y, int w, int h) {
  rect->x = x;
  rect->y = y;
  rect->w = w * scale;
  rect->h = h * scale;

  if (SDL_RenderCopy(renderer, source, NULL, rect) != 0)
    printf("%s", SDL_GetError());
}

void SDLGraphics::blit(SDL_Surface* source, int x, int y, int w, int h) {
  rect->x = x;
  rect->y = y;
  rect->w = w * scale;
  rect->h = h * scale;

  SDL_BlitSurface(source, NULL, surface, rect);
}

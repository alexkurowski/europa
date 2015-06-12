#include "graphics.h"
#include <stdio.h>

Graphics* Graphics::_instance = NULL;

Graphics::Graphics() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    throw "SDL_Init Error";
}

Graphics::~Graphics() {
  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

Graphics* Graphics::I() {
  if (!_instance)
    _instance = new Graphics();

  return _instance;
}

void Graphics::setup(int width, int height, const char* title) {
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

//=============================================================================

bool Graphics::cap() {
  currentTime = SDL_GetTicks();
  if (currentTime < lastTime + framerateTime) return true;

  _dt = ((float)(currentTime - lastTime)) / 1000;
  if (_dt > maxdt) _dt = maxdt;
  lastTime = currentTime;
  return false;
}

float Graphics::dt() {
  return _dt;
}

//=============================================================================

void Graphics::beginDraw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void Graphics::endDraw() {
  SDL_RenderPresent(renderer);
}

//=============================================================================

void Graphics::setScale() {
  scale = fmin(currentSize.w / (double)originalSize.w,
               currentSize.h / (double)originalSize.h);
}

float Graphics::getScale() {
  return scale;
}

float Graphics::originalScale() {
  return 1 / scale;
}

int Graphics::originalWidth()  { return originalSize.w; }
int Graphics::originalHeight() { return originalSize.h; }
int Graphics::currentWidth()   { return currentSize.w; }
int Graphics::currentHeight()  { return currentSize.h; }

//=============================================================================

SDL_Texture* Graphics::loadTexture(const char* path) {
  return IMG_LoadTexture(renderer, path);
}

void Graphics::setSize(Texture* texture) {
  SDL_QueryTexture(texture->texture, NULL, NULL, &texture->rect.w, &texture->rect.h);
}

void Graphics::setSize(SDL_Texture* texture, SDL_Rect &rect) {
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

//=============================================================================

void Graphics::setColor(int r, int g, int b, int a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Graphics::line(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1 * scale,
                               y1 * scale,
                               x2 * scale,
                               y2 * scale);
}

void Graphics::draw(SDL_Texture* source, int x, int y, int w, int h) {
  rect->x = x * scale;
  rect->y = y * scale;
  rect->w = w * scale;
  rect->h = h * scale;

  if (SDL_RenderCopy(renderer, source, NULL, rect) != 0)
    printf("%s\n", SDL_GetError());
}

void Graphics::draw(SDL_Texture* source, SDL_Rect* r) {
  rect->x = r->x * scale;
  rect->y = r->y * scale;
  rect->w = r->w * scale;
  rect->h = r->h * scale;

  if (SDL_RenderCopy(renderer, source, NULL, rect) != 0)
    printf("%s\n", SDL_GetError());
}

void Graphics::blit(SDL_Surface* source, int x, int y, int w, int h) {
  rect->x = x * scale;
  rect->y = y * scale;
  rect->w = w;
  rect->h = h;

  SDL_BlitSurface(source, NULL, surface, rect);
}

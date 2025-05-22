#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, int width, int height);

#endif

#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sdl_texture.h"
#include "core_wrapper.h"
#include <stdlib.h>

#define CELL_SIZE 10;

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, Textures *textures);

#endif

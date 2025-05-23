#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sdl_texture_wrapper.h"
#include "core_wrapper.h"
#include <stdlib.h>

#define CELL_SIZE 48
#define COLS 11
#define ROWS 12
#define WIDTH COLS *CELL_SIZE
#define HEIGHT ROWS *CELL_SIZE
#define flipY(y) (ROWS - 1 - (y))
#define CAR_MAX_SIZE 6

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, int width, int height);
int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, Textures *textures);
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y, int xOffset, int yOffset, SDL_RendererFlip flip);

#endif

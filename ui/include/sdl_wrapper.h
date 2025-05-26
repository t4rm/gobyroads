#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include "sdl_texture_wrapper.h"
#include "sdl_render_wrapper.h"
#include "core_wrapper.h"

#define CELL_SIZE 48
#define COLS 11
#define ROWS 12
#define WIDTH COLS *CELL_SIZE
#define HEIGHT ROWS *CELL_SIZE
#define flipY(y) (ROWS - 1 - (y))
#define CAR_MAX_SIZE 5

typedef struct _TTF_Fonts
{
    TTF_Font *small, *medium, *large;
} TTF_Fonts;

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, TTF_Fonts **fonts, int width, int height);
int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, TextureCollection *textures, TTF_Fonts *fonts);
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y, int xOffset, int yOffset, SDL_RendererFlip flip, int yDepth, int cellSize, int xDepth);
void SDLW_UpdateCars(SDL_Renderer *r, TextureCollection *t, CarQueue *queue, int y, Occupation desiredType);
int compareCarElements(const void *a, const void *b);
void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text);

#endif

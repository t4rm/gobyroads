#ifndef SDLW_RENDER_H
#define SDLW_RENDER_H

#include "sdl_wrapper.h"

typedef enum
{
    WHITE,
    GREEN
} SDLW_COLORS;

void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y, int xOffset, int yOffset, SDL_RendererFlip flip, int yDepth, int cellSize, int xDepth, int optionalSpriteSize);
void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text, SDLW_COLORS color);

#endif

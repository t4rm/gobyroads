#ifndef SDLW_RENDER_H
#define SDLW_RENDER_H

#include "sdl_wrapper.h"

void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y, int xOffset, int yOffset, SDL_RendererFlip flip, int yDepth, int cellSize, int xDepth);
void SDLW_RenderText(int x, int y, int w, int h, TTF_Font * font, SDL_Renderer * renderer, char * text);

#endif

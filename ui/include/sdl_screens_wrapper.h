#ifndef SDLW_SCREENS_H
#define SDLW_SCREENS_H

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "sdl_render_wrapper.h"
#include "core_wrapper.h"
#include <stdio.h>
#include <string.h>

void SDLW_GameOverScreen(SDL_Renderer *renderer, TTF_Fonts *fonts, int score);
void SDLW_IntroScreen(SDL_Renderer *renderer, TTF_Fonts *fonts);
void SDLW_MenuScreen(SDL_Renderer *renderer, TTF_Fonts *fonts, MenuHandler *menuHandler);

#endif
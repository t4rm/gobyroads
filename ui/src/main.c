#define SDL_MAIN_HANDLED

// SDL 2
#include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
#include "sdl_wrapper.h"
#include "sdl_texture.h"
// Core game features, directly from core
#include "gamestate.h"
#include "player.h"
#include "effect.h"
#include "car.h"
// Standard
#include <stdio.h>

// Windows dimension constants
const int WIDTH = 640;
const int HEIGHT = 480;

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDLW_Initialize(&window, &renderer, WIDTH, HEIGHT) != 0)
        exit(EXIT_FAILURE);

    Textures *textures = initTextures(renderer);

    // code gestion affichage joueur
    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(textures->playerTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }
    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 4;
    // ---

    SDL_Event event;
    int running = 1;
    int offsetX = 0, offsetY = 0;
    int distance = 0;

    while (running)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    distance += 1;
                    offsetY = 2;
                    offsetX += 1;
                    offsetX %= 4;
                    break;
                }
                break;
            case SDL_KEYUP:
                offsetX = 0;
                offsetY = 0;
                break;
            }

            SDL_Color const BACKGROUND_COLOR = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = SDL_ALPHA_OPAQUE};

            if (SDL_SetRenderDrawColor(renderer,
                                       BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());

            if (SDL_RenderClear(renderer))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());

            SDL_Rect spriteRect = {.x = offsetX * spriteWidth, .y = offsetY * spriteWidth, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = 0, .y = distance * 10, .w = spriteWidth, .h = spriteHeight};
            if (SDL_RenderCopy(renderer, textures->playerTexture, &spriteRect, &destRect))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());

            SDL_RenderPresent(renderer);
        }
    }

    destroyTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
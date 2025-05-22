#include "sdl_wrapper.h"
#include <stdlib.h>

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        return -1;
    }
    atexit(SDL_Quit);

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in image init: %s", IMG_GetError());
        return -1;
    }
    atexit(IMG_Quit);

    *window = SDL_CreateWindow("Goby Roads", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width, height, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

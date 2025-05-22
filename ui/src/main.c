#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

// Windows dimension constants
const int WIDTH = 640;
const int HEIGHT = 480;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in image init: %s", IMG_GetError());
        exit(-1);
    }
    atexit(IMG_Quit);

    SDL_Window *window;
    window = SDL_CreateWindow("demo sprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Surface *spriteSurface = IMG_Load("assets/sprite.png");
    if (!spriteSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", IMG_GetError());
        exit(-1);
    }

    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if (!spriteTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_FreeSurface(spriteSurface);

    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 4;

    SDL_Event event;
    int running = 1;
    int offset = 0;
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
                    offset += 1;
                    offset %= 4;
                    break;
                }
                break;
            case SDL_KEYUP:
                offset = 0;
                break;
            }

            SDL_Color const BACKGROUND_COLOR = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = SDL_ALPHA_OPAQUE};

            if (SDL_SetRenderDrawColor(renderer,
                                       BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());

            if (SDL_RenderClear(renderer))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());

            SDL_Rect spriteRect = {.x = offset * spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = 0, .y = distance * 10, .w = spriteWidth, .h = spriteHeight};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
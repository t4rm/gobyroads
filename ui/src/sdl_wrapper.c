#include "sdl_wrapper.h"

int gridToScreenY(int gridY, int gridHeight)
{
    return (gridHeight - 1 - gridY) * CELL_SIZE;
}

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

int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, Textures *textures)
// SDL_Texture *playerTexture, int offsetX, int offsetY, int spriteWidth, int spriteHeight, int distance
{
    // Background
    SDL_Color const BACKGROUND_COLOR = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = SDL_ALPHA_OPAQUE};

    if (SDL_SetRenderDrawColor(renderer,
                               BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());
        exit(-1);
    }

    if (SDL_RenderClear(renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());
        exit(-1);
    }

    // Map elements (Tree, SafeZone, Roads)

    // Mobs (Logs, Cars)

    // Player
    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(textures->playerTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    } // Better : Only ask for dimensions once and store them somewhere.

    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 4;

    SDL_Rect spriteRect = {
        .x = uiGs->playerOffset->x * spriteWidth,
        .y = uiGs->playerOffset->y * spriteHeight,
        .w = spriteWidth,
        .h = spriteHeight};

    SDL_Rect destRect = {
        .x = uiGs->core->player->x * 10,
        .y = gridToScreenY(uiGs->core->player->y, uiGs->core->grid->height),
        .w = spriteWidth,
        .h = spriteHeight};

    if (SDL_RenderCopy(renderer, textures->playerTexture, &spriteRect, &destRect))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        exit(-1);
    }

    SDL_RenderPresent(renderer);
    return 0;
}

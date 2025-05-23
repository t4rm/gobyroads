#include "sdl_wrapper.h"

int SDLW_Initialize(SDL_Window *window, SDL_Renderer *renderer, int width, int height)
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

    window = SDL_CreateWindow("Goby Roads", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer renderer, Textures *textures)
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

    Grid *grid = uiGs->core->grid;
    int carMaxSize = uiGs->core->carMaxSize;
    for (int i = 0; i < grid->height; i++)
    {
        int row = grid->height - 1 - i;

        for (int j = carMaxSize; j < grid->length - carMaxSize + 1; j++)
        {
            // int virtualJ = j - carMaxSize;
            // Display the map without objects :
            switch (grid->cases[row][j])
            {
                // case SAFE:
                // break;
                // case ROAD: printf("_"); break;
                // case WATER: printf("~"); break;
                // case SAFE: printf("="); break;
                // case TREE: printf("T"); break;
                // case CAR_LEFT: printf("<"); break;
                // case CAR_RIGHT: printf(">"); break;
                // case LOG: printf("L"); break;
                // default: printf("?"); break;
            }
            // if (row == playerY && j == playerX)
            // uiGs->playerOffset->
        }
    }

    // Debug informations for Player true and virtual coordinates
    // printf("%d, %d (%d)\n", uiGs->core->player->x, uiGs->core->player->y, flipY(uiGs->core->player->y));
    SDLW_RenderCopy(renderer, textures->playerTexture, uiGs->core->player->x, uiGs->core->player->y, uiGs->playerOffset->x, uiGs->playerOffset->y);
    SDLW_RenderCopy(renderer, textures->safeTexture, 400, 400, 1, 1);

    SDL_RenderPresent(renderer);
    return 0;
}

// This work fine for dynamic sprite like Player, but this is "OVERKILL" for Static Sprites like Grass.
// Grass don't have offsets, but we could ! They would become animated (simulating the wind for example)
// Same for waves in Rivers
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y, int xOffset, int yOffset)
{
    // The whole spritesheet :
    SDL_Rect spriteRect = {xOffset * CELL_SIZE, yOffset * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    // Better : Only define it once for static spritesheet (only 1 sprite in the sheet)
    // The part we need, often its the whole spritesheet but in the case of a player it's only a portion of it.
    SDL_Rect destRect = {x * CELL_SIZE, flipY(y) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    if (SDL_RenderCopy(r, t, &spriteRect, &destRect))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
}
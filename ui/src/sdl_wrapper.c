#include "sdl_wrapper.h"

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

int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, TextureCollection *textures)
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

    Grid *grid = uiGs->core->grid;
    int carMaxSize = uiGs->core->carMaxSize;
    for (int y = grid->height - 1; y >= 0; y--)
    {
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            // Map elements (SafeZone, Roads) (Background)
            switch (grid->cases[y][x])
            {
            case SAFE:
            case TREE:
                SDLW_RenderCopy(renderer, GetTexture(textures, "grass"), x, y, 0, 0, SDL_FLIP_NONE, 0);
                break;
            case ROAD:
            case CAR_LEFT:
            case CAR_RIGHT:
                SDLW_RenderCopy(renderer, GetTexture(textures, "road"), x, y, 0, 0, SDL_FLIP_NONE, 0);
                break;
            case WATER:
            case LOG:
                SDLW_RenderCopy(renderer, GetTexture(textures, "water"), x, y, 0, 0, SDL_FLIP_NONE, 0);
                break;
            default:
                break;
            }

            // Obsolète, ne permet pas d'afficher des voitures plus longues différement de voitures d'une seule case.
            // Mobs (Logs, Cars, Trees) (Foreground)
            // switch (grid->cases[y][x])
            // {
            // case TREE:
            //     SDLW_RenderCopy(renderer, textures->treeTexture, x, y, 0, 0, SDL_FLIP_NONE, 12);
            //     break;
            // case CAR_LEFT:
            //     SDLW_RenderCopy(renderer, textures->carTexture, x, y, 0, 0, SDL_FLIP_HORIZONTAL, 8);
            //     break;
            // case CAR_RIGHT:
            //     SDLW_RenderCopy(renderer, textures->carTexture, x, y, 0, 0, SDL_FLIP_NONE, 8);
            //     break;
            // case LOG:
            //     SDLW_RenderCopy(renderer, textures->logTexture, x, y, 0, 0, SDL_FLIP_NONE, 0);
            //     break;
            // case SAFE:
            // case ROAD:
            // case WATER:
            //     break;
            // }
        }
    }

    CarElement *h = uiGs->core->cars->head;
    while (h != NULL && h->car != NULL)
    {
        Car *c = h->car;
        switch (c->size)
        {
        case 1:
            SDLW_RenderCopy(renderer, GetTexture(textures, "car_1"), c->x, c->y, 0, 0, SDL_FLIP_NONE, 0);

            break;
        case 2:

            break;
        case 3:

            break;
        default:
            break;
        }

        h = h->next;
    }

    // printf("\n%d, %d (%d)  : %d\n", h->car->x, h->car->y, flipY(h->car->y), h->car->size);
    // printf("%d, %d (%d)\n", uiGs->core->player->x, uiGs->core->player->y, flipY(uiGs->core->player->y));
    // Player
    // Debug informations for Player true and virtual coordinates
    // printf("%d, %d\n", grid->height, grid->length);
    SDLW_RenderCopy(renderer, GetTexture(textures, "player"), uiGs->core->player->x, uiGs->core->player->y, uiGs->playerOffset->x, uiGs->playerOffset->y, SDL_FLIP_NONE, 0);
    SDL_RenderPresent(renderer);
    return 0;
}

// This work fine for dynamic sprite like Player, but this is "OVERKILL" for Static Sprites like Grass.
// Grass don't have offsets, but we could ! They would become animated (simulating the wind for example)
// Same for waves in Rivers
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y,
                     int xOffset, int yOffset, SDL_RendererFlip flip,
                     int yDepth)
{
    // The whole spritesheet :
    SDL_Rect spriteRect = {xOffset * CELL_SIZE, yOffset * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    // Better : Only define it once for static spritesheet (only 1 sprite in the sheet)
    // The part we need, often its the whole spritesheet but in the case of a player it's only a portion of it.
    SDL_Rect destRect = {(x - CAR_MAX_SIZE - 1) * CELL_SIZE, flipY(y) * CELL_SIZE - yDepth, CELL_SIZE, CELL_SIZE};
    if (SDL_RenderCopyEx(r, t, &spriteRect, &destRect, 0.0, NULL, flip))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
}
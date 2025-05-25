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
    SDL_Color const BACKGROUND_COLOR = {255, 255, 255, SDL_ALPHA_OPAQUE};
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);

    Grid *grid = uiGs->core->grid;
    CarQueue *carQueue = uiGs->core->cars;
    int carMaxSize = uiGs->core->carMaxSize;

    for (int y = grid->height - 1; y >= 0; y--)
    {
        // -------- Sol --------
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            switch (grid->cases[y][x])
            {
            case SAFE:
            case TREE:
                SDLW_RenderCopy(renderer, GetTexture(textures, "grass"), x, y, x % 5, 0, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
                break;
            case ROAD:
            case CAR_LEFT:
            case CAR_RIGHT:
                SDLW_RenderCopy(renderer, GetTexture(textures, "road"), x, y, 0, 0, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
                break;
            case WATER:
            case LOG:
                SDLW_RenderCopy(renderer, GetTexture(textures, "water"), x, y, x % 3, 0, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
                break;
            default:
                break;
            }
        }

        if (uiGs->core->player->y == y)
        {
            // -------- Joueur --------
            SDLW_RenderCopy(renderer, GetTexture(textures, "player"), uiGs->core->player->x, uiGs->core->player->y, uiGs->playerOffset->x, uiGs->playerOffset->y, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
        }

        // -------- Voitures --------
        for (CarElement *carElt = carQueue->head; carElt != NULL; carElt = carElt->next)
        {
            Car *c = carElt->car;
            if (!c || c->type == LOG || c->y != y)
                continue;

            int centerX = c->x + c->direction * (c->size - 1) / 2;
            int xDepth = 0;
            SDL_RendererFlip flip = (c->direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            int spriteSize, yDepth;
            const char *textureName;

            switch (c->size)
            {
            case 1:
                textureName = "car_1";
                spriteSize = 100;
                yDepth = 4;
                break;
            case 2:
                textureName = "car_2";
                spriteSize = 100;
                yDepth = 8;
                break;
            case 3:
                textureName = "car_3";
                spriteSize = 140;
                yDepth = 12;
                break;
            case 4:
                textureName = "car_4";
                spriteSize = 210;
                yDepth = 12;
                xDepth = 0.5 * CELL_SIZE;
                break;
            case 5:
                textureName = "car_5";
                spriteSize = 230;
                yDepth = 12;
                break;
            default:
                textureName = "car_1";
                spriteSize = 48;
                yDepth = 12;
                break;
            }

            SDLW_RenderCopy(renderer, GetTexture(textures, textureName),
                            centerX, c->y, c->accumulator % 4, c->accumulator % 3,
                            flip, yDepth, spriteSize, xDepth);
        }

        // -------- Arbres --------
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            if (grid->cases[y][x] == TREE)
            {
                SDLW_RenderCopy(renderer, GetTexture(textures, "tree"), x, y, 0, 0, SDL_FLIP_NONE, 14, 128, 0);
            }
        }
    }

    SDL_RenderPresent(renderer);
    return 0;
}

int compareCarElements(const void *a, const void *b)
{
    CarElement *carA = *(CarElement **)a;
    CarElement *carB = *(CarElement **)b;

    return carB->car->y - carA->car->y;
}

// We use a temporary structure because we need to sort by Y Axis : The render should be from the top to the bottom for a top down view.
void SDLW_RenderCarsSortedByY(SDL_Renderer *renderer, TextureCollection *textures, CarQueue *queue)
{
    if (queue->size == 0)
        return;

    CarElement **carArray = malloc(sizeof(CarElement *) * queue->size);
    if (!carArray)
        return;

    int i = 0;
    CarElement *cursor = queue->head;
    while (cursor != NULL && i < queue->size)
    {
        carArray[i++] = cursor;
        cursor = cursor->next;
    }

    qsort(carArray, queue->size, sizeof(CarElement *), compareCarElements);

    for (int j = 0; j < queue->size; j++)
    {
        Car *c = carArray[j]->car;

        if (!c || c->type == LOG)
            break;

        int centerX = c->x + c->direction * (c->size - 1) / 2;
        int xDepth = 0;
        SDL_RendererFlip flip = (c->direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        int spriteSize, yDepth;
        const char *textureName;

        switch (c->size)
        {
        case 1:
            textureName = "car_1";
            spriteSize = 100;
            yDepth = 4;
            break;
        case 2:
            textureName = "car_2";
            spriteSize = 100;
            yDepth = 8;
            break;
        case 3:
            textureName = "car_3";
            spriteSize = 140;
            yDepth = 12;
            break;
        case 4:
            textureName = "car_4";
            spriteSize = 210;
            yDepth = 12;
            xDepth = 0.5 * CELL_SIZE;
            break;
        case 5:
            textureName = "car_5";
            spriteSize = 230;
            yDepth = 12;
            break;
        default:
            textureName = "car_1";
            spriteSize = 48;
            yDepth = 12;
            break;
        }

        SDLW_RenderCopy(renderer, GetTexture(textures, textureName),
                        centerX, c->y, c->accumulator % 4, c->accumulator % 3, flip, yDepth, spriteSize, xDepth);
    }

    free(carArray);
}

// This work fine for dynamic sprite like Player, but this is "OVERKILL" for Static Sprites like Grass.
// Grass don't have offsets, but we could ! They would become animated (simulating the wind for example)
// Same for waves in Rivers
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y,
                     int xOffset, int yOffset, SDL_RendererFlip flip,
                     int yDepth, int spriteSize, int xDepth)
{
    SDL_Rect spriteRect = {
        xOffset * spriteSize,
        yOffset * spriteSize,
        spriteSize,
        spriteSize};

    // Position logique dans le monde (coordonnées logiques de la grille)
    int logicalX = (x - CAR_MAX_SIZE - 1) * CELL_SIZE + xDepth;
    int logicalY = flipY(y) * CELL_SIZE - yDepth;

    // Décalage pour centrer un sprite plus grand que CELL_SIZE (48)
    int offsetX = (spriteSize - CELL_SIZE) / 2;
    int offsetY = (spriteSize - CELL_SIZE) / 2;

    SDL_Rect destRect = {
        logicalX - offsetX,
        logicalY - offsetY,
        spriteSize,
        spriteSize};

    if (SDL_RenderCopyEx(r, t, &spriteRect, &destRect, 0.0, NULL, flip))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
    }
}

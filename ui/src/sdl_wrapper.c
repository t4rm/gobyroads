#include "sdl_wrapper.h"

int compareCarElements(const void *a, const void *b)
{
    CarElement *carA = *(CarElement **)a;
    CarElement *carB = *(CarElement **)b;

    return carB->car->y - carA->car->y;
}

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font, int width, int height)
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

    if (TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in ttf init: %s", SDL_GetError());
    }

    *font = TTF_OpenFont("fonts/retro-pixel-thick.ttf", 20);
    if (font == NULL)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in ttf opening: %s", SDL_GetError());

    return 0;
}

int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, TextureCollection *textures, TTF_Font *font)
{
    SDL_Color const BACKGROUND_COLOR = {255, 255, 255, SDL_ALPHA_OPAQUE};
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);

    Grid *grid = uiGs->core->grid;
    CarQueue *carQueue = uiGs->core->cars;
    int carMaxSize = uiGs->core->carMaxSize;

    for (int y = grid->height - 1; y >= 0; y--)
    {
        SDL_Texture *t = NULL;
        int xOffset = 0, yOffset = 0;
        // -------- Sol --------
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            switch (grid->cases[y][x])
            {
            case SAFE:
            case TREE:
                t = GetTexture(textures, "grass");
                xOffset = x % 5;
                break;
            case ROAD:
            case CAR_LEFT:
            case CAR_RIGHT:
                t = GetTexture(textures, "road");
                break;
            case WATER:
            case LOG:
                t = GetTexture(textures, "water");
                xOffset = x % 3;
                break;
            default:
                break;
            }
            SDLW_RenderCopy(renderer, t, x, y, xOffset, yOffset, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
        }
        // -------- Rondins  --------
        SDLW_RenderCars(renderer, textures, carQueue, y, WATER);
        // -------- Joueur --------
        if (uiGs->core->player->y == y)
            SDLW_RenderCopy(renderer, GetTexture(textures, "player"), uiGs->core->player->x, uiGs->core->player->y, uiGs->playerOffset->x, uiGs->playerOffset->y, SDL_FLIP_NONE, 0, CELL_SIZE, 0);
        // -------- Voitures  --------
        SDLW_RenderCars(renderer, textures, carQueue, y, ROAD);
        // -------- Arbres --------x
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            if (grid->cases[y][x] == TREE)
                SDLW_RenderCopy(renderer, GetTexture(textures, "tree"), x, y, 0, 0, SDL_FLIP_NONE, 14, 128, 0);
        }
    }


    // Print score :
    char scoreChar[3];
    sprintf(scoreChar, "%d", uiGs->core->score);
    SDLW_RenderText(CELL_SIZE *0.15, 0 - CELL_SIZE * 0.15, 30*strlen(scoreChar), 60, font, renderer, scoreChar);
    // Print game title
    // --
    SDL_RenderPresent(renderer);
    return 0;
}

void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text)
{
    SDL_Rect rectangleTitle = {x, y, w, h};

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleTitle);
}

void SDLW_RenderCars(SDL_Renderer *r, TextureCollection *t, CarQueue *queue, int y, Occupation desiredType)
{
    for (CarElement *carElt = queue->head; carElt != NULL; carElt = carElt->next)
    {
        Car *c = carElt->car;
        if (!c || c->y != y || c->type != desiredType)
            continue;

        const char *textureName = NULL;
        int spriteSize = 0, yDepth = 0, xDepth = 0, yOffset = c->type == WATER ? 0 : c->accumulator % 3;

        if (c->type == WATER)
            switch (c->size)
            {
            case 1:
                textureName = "l_1";
                spriteSize = 48;
                break;
            case 2:
                textureName = "l_2";
                spriteSize = 96;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 3:
                textureName = "l_3";
                spriteSize = 144;
                break;
            case 4:
                textureName = "l_4";
                spriteSize = 192;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 5:
                textureName = "l_5";
                spriteSize = 240;
                break;
            default:
                textureName = NULL;
                break;
            }
        else
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
                xDepth = c->direction * 0.5 * CELL_SIZE;
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
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 5:
                textureName = "car_5";
                spriteSize = 230;
                yDepth = 12;
                break;
            default:
                textureName = NULL;
                break;
            }

        if (!textureName)
            continue;

        int centerX = c->x + c->direction * (c->size - 1) / 2;
        SDL_RendererFlip flip = (c->direction == -1 && c->type != WATER) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDLW_RenderCopy(r, GetTexture(t, textureName), centerX, c->y, c->accumulator % 4, yOffset, flip, yDepth, spriteSize, xDepth);
    }
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

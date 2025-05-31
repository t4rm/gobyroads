#include "sdl_wrapper.h"

const char *CarColorNames[COLOR_COUNT] = {
    "black", "blue", "brown", "green", "magenta", "red", "white", "yellow"};

bool isRoadOrCar(Occupation type)
{
    return type == ROAD || type == CAR_LEFT || type == CAR_RIGHT;
}

int compareCarElements(const void *a, const void *b)
{
    CarElement *carA = *(CarElement **)a;
    CarElement *carB = *(CarElement **)b;

    return carB->car->y - carA->car->y;
}

int SDLW_Initialize(SDL_Window **window, SDL_Renderer **renderer, TTF_Fonts **fonts, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) // Plutôt init les 4 qu'on use non ?
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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in ttf init: %s", SDL_GetError());

    (*fonts)->small = TTF_OpenFont("fonts/retro-pixel-thick.ttf", 32);
    (*fonts)->medium = TTF_OpenFont("fonts/retro-pixel-thick.ttf", 48);
    (*fonts)->large = TTF_OpenFont("fonts/retro-pixel-thick.ttf", 62);

    if ((*fonts)->large == NULL || (*fonts)->medium == NULL || (*fonts)->small == NULL)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in ttf opening: %s", SDL_GetError());

    return 0;
}

int SDLW_UpdateAndRender(UIGameState *uiGs, SDL_Renderer *renderer, TextureCollection *textures, TTF_Fonts *fonts)
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
            case TRAIN:
            case WARNING:
            case RAIL:
            case SAFE:
            case TREE:
                t = GetTexture(textures, "grass");
                xOffset = x % 5;
                break;
            case ROAD:
            case CAR_LEFT:
            case CAR_RIGHT:
                t = GetTexture(textures, "road");
                bool belowIsRoadOrCar = false;
                bool aboveIsRoadOrCar = false;

                if (y > 0)
                    belowIsRoadOrCar = isRoadOrCar(grid->cases[y - 1][0]);

                if (y + 1 < grid->height)
                    aboveIsRoadOrCar = isRoadOrCar(grid->cases[y + 1][0]);

                xOffset = !belowIsRoadOrCar ? 0 : !aboveIsRoadOrCar ? 2
                                                                    : 1;
                break;
            case WATER:
            case LOG:
                t = GetTexture(textures, "water");
                xOffset = x % 3;
                break;
            case ICE:
                t = GetTexture(textures, "ice");
                bool belowIsIce = false;
                bool aboveIsIce = false;

                if (y > 0)
                    belowIsIce = grid->cases[y - 1][0] == ICE;

                if (y + 1 < grid->height)
                    aboveIsIce = grid->cases[y + 1][0] == ICE;

                xOffset = !belowIsIce ? 0 : !aboveIsIce ? 2
                                                        : 1;
                break;
            default:
                break;
            }
            SDLW_RenderCopy(renderer, t, x, y, xOffset, yOffset, SDL_FLIP_NONE, 0, CELL_SIZE, 0, 0);
        }

        // Sur-couche du sol
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            if (grid->cases[y][x] == TRAIN || grid->cases[y][x] == RAIL || grid->cases[y][x] == WARNING)
                SDLW_RenderCopy(renderer, GetTexture(textures, "rail"), x, y, 0, 0, SDL_FLIP_NONE, 0, CELL_SIZE, 0, 0);
        }

        // -------- Rondins  --------
        SDLW_UpdateCars(renderer, textures, carQueue, y, WATER, uiGs->core->grid->rowManagers[y]);
        // -------- Joueur --------
        if (uiGs->core->player->y == y)
            SDLW_RenderCopy(renderer, GetTexture(textures, "player"), uiGs->core->player->x, uiGs->core->player->y, uiGs->playerOffset->x, uiGs->playerOffset->y, SDL_FLIP_NONE, 0, CELL_SIZE, 0, 0);

        // -------- Voitures  --------
        SDLW_UpdateCars(renderer, textures, carQueue, y, ROAD, uiGs->core->grid->rowManagers[y]);
        // -------- Arbres & Rails & Warning--------x
        for (int x = carMaxSize; x < grid->length - carMaxSize + 1; x++)
        {
            if (grid->cases[y][x] == TREE)
                SDLW_RenderCopy(renderer, GetTexture(textures, "tree"), x, y, 0, 0, SDL_FLIP_NONE, 14, 128, 0, 0);
            else if (grid->cases[y][x] == WARNING)
                SDLW_RenderCopy(renderer, GetTexture(textures, "warning"), x, y, 0, 0, SDL_FLIP_NONE, 0, CELL_SIZE, 0, 0);
        }

        if (grid->cases[y][0] == TRAIN)
            SDLW_RenderCopy(renderer, GetTexture(textures, "trains"), grid->length / 2, y, 0, grid->rowManagers[y]->cooldown % 5, SDL_FLIP_NONE, 18, 530, 0, 72, "");
    }

    char scoreChar[3];
    sprintf(scoreChar, "%d", uiGs->core->score);
    SDLW_RenderText(CELL_SIZE * 0.15, 0 - CELL_SIZE * 0.15, 30 * strlen(scoreChar), 60, fonts->medium, renderer, scoreChar);
    SDL_RenderPresent(renderer);
    return 0;
}

void SDLW_UpdateCars(SDL_Renderer *r, TextureCollection *t, CarQueue *queue, int y, Occupation desiredType, RowManager *rm)
{
    for (CarElement *carElt = queue->head; carElt != NULL; carElt = carElt->next)
    {
        Car *c = carElt->car;
        if (!c || c->y != y || rm->type != desiredType)
            continue;

        char textureName[50];
        int spriteSize = 0, yDepth = 0, xDepth = 0, yOffset = rm->type == WATER ? 0 : rm->cooldown % 3;

        if (rm->type == WATER)
            switch (c->size)
            {
            case 1:
                strcpy(textureName, "l_1");
                spriteSize = 48;
                break;
            case 2:
                strcpy(textureName, "l_2");
                spriteSize = 96;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 3:
                strcpy(textureName, "l_3");
                spriteSize = 144;
                break;
            case 4:
                strcpy(textureName, "l_4");
                spriteSize = 192;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 5:
                strcpy(textureName, "l_5");
                spriteSize = 240;
                break;
            default:
                strcpy(textureName, "l_1");
                break;
            }
        else
        {
            if (c->variant != 'c' && c->variant != 'g' && c->size != 5) // c & g are the only static variant, hardcoded but manageable. Side 5 is a static too.
                if (c->variant == '\0')
                    snprintf(textureName, sizeof(textureName), "car_%d_%s", c->size, CarColorNames[c->color]);
                else
                    snprintf(textureName, sizeof(textureName), "car_%d%c_%s", c->size, c->variant, CarColorNames[c->color]);

            switch (c->size)
            {
            case 1:
                spriteSize = 100;
                yDepth = 4;
                break;
            case 2:
                if (c->variant == 'c')
                    strcpy(textureName, "taxi");
                else if (c->variant == 'g')
                    strcpy(textureName, "police");
                spriteSize = 100;
                yDepth = 8;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 3:
                if (c->variant == 'c')
                    strcpy(textureName, "ambulance");
                spriteSize = 140;
                yDepth = 12;
                break;
            case 4:
                spriteSize = 210;
                yDepth = 12;
                xDepth = c->direction * 0.5 * CELL_SIZE;
                break;
            case 5:
                strcpy(textureName, "limo");
                spriteSize = 230;
                yDepth = 8;
                break;
            default:
                textureName[0] = '\0';
                break;
            }
        }

        if (textureName[0] == '\0')
            continue;

        int centerX = c->x + c->direction * (c->size - 1) / 2;
        SDL_RendererFlip flip = (c->direction == -1 && rm->type != WATER) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDLW_RenderCopy(r, GetTexture(t, textureName), centerX, c->y, rm->cooldown % 4, yOffset, flip, yDepth, spriteSize, xDepth, 0);
    }
}
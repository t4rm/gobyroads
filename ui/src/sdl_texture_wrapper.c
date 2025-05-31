#include "sdl_texture_wrapper.h"

static const char *CAR_COLORS[] = {"black", "blue", "brown", "green", "magenta", "red", "white", "yellow"}; // Possibilité de fusionner avec le tableau dans sdl_wrapper.c

static const CarConfig CAR_CONFIGS[] = {
    {1, '\0', CAR_COLORS, 8, NULL, NULL},

    {2, 'a', CAR_COLORS, 8, NULL, NULL},
    {2, 'b', CAR_COLORS, 8, NULL, NULL},
    {2, 'c', NULL, 0, "taxi", "assets/sprites/cars/2/c/taxi.png"},
    {2, 'd', CAR_COLORS, 8, NULL, NULL},
    {2, 'e', CAR_COLORS, 8, NULL, NULL},
    {2, 'f', CAR_COLORS, 8, NULL, NULL},
    {2, 'g', NULL, 0, "police", "assets/sprites/cars/2/g/police.png"},

    {3, 'a', CAR_COLORS, 8, NULL, NULL},
    {3, 'b', CAR_COLORS, 8, NULL, NULL},
    {3, 'c', NULL, 0, "ambulance", "assets/sprites/cars/3/c/ambulance.png"},
    {3, 'd', CAR_COLORS, 8, NULL, NULL},

    {4, '\0', CAR_COLORS, 8, NULL, NULL},
    {5, '\0', NULL, 0, "limo", "assets/sprites/cars/5/white.png"},
};

// Textures de base (statiques)
static const TextureInfo BASE_TEXTURES[] = {
    {"grass", "assets/sprites/grass.png"},
    {"road", "assets/sprites/road.png"},
    {"water", "assets/sprites/water.png"},
    {"l_1", "assets/sprites/logs/1.png"},
    {"l_2", "assets/sprites/logs/2.png"},
    {"l_3", "assets/sprites/logs/3.png"},
    {"l_4", "assets/sprites/logs/4.png"},
    {"l_5", "assets/sprites/logs/5.png"},
    {"player", "assets/sprites/goblin.png"},
    {"ice", "assets/sprites/ice.png"},
    {"rail", "assets/sprites/rail.png"},
    {"trains", "assets/sprites/trains.png"},
    {"warning", "assets/sprites/warning.png"},
    {"tree", "assets/sprites/trees/1.png"}};

static int calculateTotalTextureCount()
{
    int baseCount = sizeof(BASE_TEXTURES) / sizeof(BASE_TEXTURES[0]);
    int carConfigCount = sizeof(CAR_CONFIGS) / sizeof(CAR_CONFIGS[0]);
    int carTextureCount = 0;

    for (int i = 0; i < carConfigCount; i++)
        if (CAR_CONFIGS[i].staticName != NULL)
            carTextureCount += 1;
        else
            carTextureCount += CAR_CONFIGS[i].colorCount;

    return baseCount + carTextureCount;
}

static void generateCarTextureName(const CarConfig *config, const char *color, char *name, size_t nameSize)
{
    if (config->variant == '\0')
        snprintf(name, nameSize, "car_%d_%s", config->size, color);
    else
        snprintf(name, nameSize, "car_%d%c_%s", config->size, config->variant, color);
}

static void generateCarTexturePath(const CarConfig *config, const char *color, char *path, size_t pathSize)
{
    if (config->variant == '\0')
        snprintf(path, pathSize, "assets/sprites/cars/%d/%s.png", config->size, color);
    else
        snprintf(path, pathSize, "assets/sprites/cars/%d/%c/%s.png", config->size, config->variant, color);
}

static void addBaseTextures(TextureCollection *collection, SDL_Renderer *renderer, int *currentIndex)
{
    int baseCount = sizeof(BASE_TEXTURES) / sizeof(BASE_TEXTURES[0]);

    for (int i = 0; i < baseCount; i++)
    {
        collection->textures[*currentIndex].name = strdup(BASE_TEXTURES[i].name);
        collection->textures[*currentIndex].texture = SDLW_MakeTexture((char *)BASE_TEXTURES[i].path, renderer);
        (*currentIndex)++;
    }
}

static void addCarTextures(TextureCollection *collection, SDL_Renderer *renderer, int *currentIndex)
{
    int carConfigCount = sizeof(CAR_CONFIGS) / sizeof(CAR_CONFIGS[0]);
    char textureName[50];
    char texturePath[100];

    for (int i = 0; i < carConfigCount; i++)
    {
        const CarConfig *config = &CAR_CONFIGS[i];

        if (config->staticName != NULL)
        {
            // Textures statiques (Taxi, Police, Ambulance)
            collection->textures[*currentIndex].name = strdup(config->staticName);
            collection->textures[*currentIndex].texture = SDLW_MakeTexture((char *)config->staticPath, renderer);
            (*currentIndex)++;
        }
        else
        {
            // Textures colorées
            for (int j = 0; j < config->colorCount; j++)
            {
                generateCarTextureName(config, config->colors[j], textureName, sizeof(textureName));
                generateCarTexturePath(config, config->colors[j], texturePath, sizeof(texturePath));

                collection->textures[*currentIndex].name = strdup(textureName);
                collection->textures[*currentIndex].texture = SDLW_MakeTexture(texturePath, renderer);
                (*currentIndex)++;
            }
        }
    }
}

TextureCollection *SDLW_InitTextures(SDL_Renderer *renderer)
{
    int totalCount = calculateTotalTextureCount();

    TextureCollection *collection = malloc(sizeof(TextureCollection));
    collection->count = totalCount;
    collection->textures = malloc(sizeof(NamedTexture) * totalCount);

    int currentIndex = 0;
    addBaseTextures(collection, renderer, &currentIndex);
    addCarTextures(collection, renderer, &currentIndex);

    return collection;
}

SDL_Texture *GetTexture(TextureCollection *collection, const char *name)
{
    for (int i = 0; i < collection->count; i++)
        if (strcmp(collection->textures[i].name, name) == 0)
            return collection->textures[i].texture;
    return NULL;
}

void SDLW_DestroyTextures(TextureCollection *collection)
{
    for (int i = 0; i < collection->count; i++)
    {
        SDL_DestroyTexture(collection->textures[i].texture);
        free(collection->textures[i].name);
    }
    free(collection->textures);
    free(collection);
}

SDL_Texture *SDLW_MakeTexture(char *spriteName, SDL_Renderer *renderer)
{
    SDL_Surface *spriteSurface = IMG_Load(spriteName);
    if (!spriteSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", IMG_GetError());
        exit(-1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_FreeSurface(spriteSurface);
    return texture;
}
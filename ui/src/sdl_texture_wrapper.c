#include "sdl_texture_wrapper.h"

static const char *CAR_COLORS[] = {"black", "blue", "brown", "green", "magenta", "red", "white", "yellow"}; // Possibility of merging with the array in sdl_wrapper.c

// Dynamic texture
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

// Base textures (static)
static const TextureInfo BASE_TEXTURES[] = {
    {"grass", "assets/sprites/grass.png"},
    {"road", "assets/sprites/road.png"},
    {"water", "assets/sprites/water.png"},
    {"speaker-m", "assets/sprites/speaker-m.png"},
    {"speaker-u", "assets/sprites/speaker-u.png"},
    {"l_1", "assets/sprites/logs/1.png"},
    {"l_2", "assets/sprites/logs/2.png"},
    {"l_3", "assets/sprites/logs/3.png"},
    {"l_4", "assets/sprites/logs/4.png"},
    {"l_5", "assets/sprites/logs/5.png"},
    {"player", "assets/sprites/goblin2.png"},
    {"ice", "assets/sprites/ice.png"},
    {"rail", "assets/sprites/rail.png"},
    {"trains", "assets/sprites/trains.png"},
    {"warning", "assets/sprites/warning.png"},
    {"tree", "assets/sprites/trees/1.png"}};

/* Get the amount of texture in total by adding the base one to the dynamic ones */
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
/* Generate the name of a texture based on its config
 * config: the config of the car texture
 * color: a string indicating the color to use
 * name: a pointer to the name of the texture
 * nameSize: the max size of this name.
 * Usage of size_t : "size_t can store the maximum size of a theoretically possible object of any type", it is used by default by our EDI.
 */
static void generateCarTextureName(const CarConfig *config, const char *color, char *name, size_t nameSize)
{
    if (config->variant == '\0')
        snprintf(name, nameSize, "car_%d_%s", config->size, color);
    else
        snprintf(name, nameSize, "car_%d%c_%s", config->size, config->variant, color);
}

/* Generate the path of a texture based on its config
 * config: the config of the car texture
 * color: a string indicating the color to use
 * path: a pointer to the path of the texture
 * pathSize: the max size of this path.
 */
static void generateCarTexturePath(const CarConfig *config, const char *color, char *path, size_t pathSize)
{
    if (config->variant == '\0')
        snprintf(path, pathSize, "assets/sprites/cars/%d/%s.png", config->size, color);
    else
        snprintf(path, pathSize, "assets/sprites/cars/%d/%c/%s.png", config->size, config->variant, color);
}

/* Add the base textures (static) to the TextureCollection
 * collection: a pointer to the collection
 * renderer: a pointer to the renderer
 * currentIndex: a pointer to the current index
 */
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
/* Add the car texture to the TextureCollection
 * collection: a pointer to the collection
 * renderer: a pointer to the renderer
 * currentIndex: a pointer to the currentIndex
 */
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
            // Static texture from CarConfig (Taxi, Police, Ambulance)
            collection->textures[*currentIndex].name = strdup(config->staticName);
            collection->textures[*currentIndex].texture = SDLW_MakeTexture((char *)config->staticPath, renderer);
            (*currentIndex)++; // We can add them in the Base textures and remove this code.
        }
        else
        {
            // Colored textures
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

/* Initialize the TextureCollection with empty values, then fill it with the base textures and the car textures
 * renderer: a pointer to the renderer
 * returns the TextureCollection
 */
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

/* Get a Texture from the TextureCollection with its name
 * collection: a pointer to the TextureCollection
 * name: a string of the name of the texture
 returns the SDL_Texture
 */
SDL_Texture *GetTexture(TextureCollection *collection, const char *name)
{
    for (int i = 0; i < collection->count; i++)
        if (strcmp(collection->textures[i].name, name) == 0)
            return collection->textures[i].texture;
    return NULL;
}

/* Free a TextureCollection
 * collection: a pointer to the TextureCollection
 */
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

/* Wrap the creation of a Texture in a function
 * spriteName: the name of the sprite we want to load
 * renderer: a pointer to the renderer
 */
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
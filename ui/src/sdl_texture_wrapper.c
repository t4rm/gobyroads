#include "sdl_texture_wrapper.h"

TextureCollection *SDLW_InitTextures(SDL_Renderer *renderer)
{
    TextureInfo textureData[] = {
        // Map :
        {"grass", "assets/grass.png"},
        {"road", "assets/road.png"},
        {"water", "assets/water.jpg"},

        // Cars (ALWAYS EAST DIRECTION) :
        {"car_1", "assets/cars/1.png"},
        {"car_2", "assets/cars/2.png"},
        {"car_3", "assets/cars/3.png"},
        {"car_4", "assets/cars/4.png"},
        {"car_5", "assets/cars/5.png"},

        // Logs :
        {"l_1", "assets/logs/1.png"},
        {"l_2", "assets/logs/2.png"},
        {"l_3", "assets/logs/3.png"},
        {"l_4", "assets/logs/4.png"},
        {"l_5", "assets/logs/5.png"},

        // Player, tree, ice, rails, train, warning
        {"player", "assets/goblin.png"},
        {"ice", "assets/ice.png"},
        {"tree", "assets/trees/1.png"}

    };

    int textureCount = sizeof(textureData) / sizeof(TextureInfo);

    TextureCollection *collection = malloc(sizeof(TextureCollection));
    collection->count = textureCount;
    collection->textures = malloc(sizeof(NamedTexture) * textureCount);

    for (int i = 0; i < textureCount; i++)
    {
        collection->textures[i].name = strdup(textureData[i].name);
        collection->textures[i].texture = SDLW_MakeTexture((char *)textureData[i].path, renderer);
    }

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

SDL_Texture *SDLW_MakeTexture(char *sprite_name, SDL_Renderer *renderer)
{
    SDL_Surface *spriteSurface = IMG_Load(sprite_name);
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

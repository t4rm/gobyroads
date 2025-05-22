#include "sdl_texture.h"

Textures *initTextures(SDL_Renderer *renderer)
{
    Textures *textures = (Textures *)malloc(sizeof(Textures));

    textures->playerTexture = makeTextures("assets/goblin.png", renderer);
    textures->safeTexture = makeTextures("assets/grass.png", renderer);
    // textures->carTexture = makeTextures("sprites/car.png", renderer);
    // textures->logTexture = makeTextures("sprites/log.png", renderer);
    // textures->roadTexture = makeTextures("sprites/grass.png", renderer);
    // textures->treeTexture = makeTextures("sprites/bush.png", renderer);
    // textures->waterTexture = makeTextures("sprites/water.png", renderer);

    return (textures);
}

void destroyTextures(Textures *textures)
{
    SDL_DestroyTexture(textures->playerTexture);
    SDL_DestroyTexture(textures->safeTexture);
    //     SDL_DestroyTexture(textures->carTexture);
    //     SDL_DestroyTexture(textures->logTexture);
    //     SDL_DestroyTexture(textures->roadTexture);
    //     SDL_DestroyTexture(textures->treeTexture);
    //     SDL_DestroyTexture(textures->waterTexture);
}

SDL_Texture *makeTextures(char *sprite_name, SDL_Renderer *renderer)
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

    return (texture);
}

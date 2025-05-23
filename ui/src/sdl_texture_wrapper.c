#include "sdl_texture_wrapper.h"

Textures *SDLW_InitTextures(SDL_Renderer *renderer)
{
    Textures *textures = (Textures *)malloc(sizeof(Textures));

    textures->safeTexture = SDLW_MakeTexture("assets/grass.png", renderer);
    textures->playerTexture = SDLW_MakeTexture("assets/goblin.png", renderer);
    textures->roadTexture = SDLW_MakeTexture("assets/road.png", renderer);
    textures->waterTexture = SDLW_MakeTexture("assets/water.png", renderer);
    textures->treeTexture = SDLW_MakeTexture("assets/tree.png", renderer);
    textures->carTexture = SDLW_MakeTexture("assets/car.png", renderer);
    textures->logTexture = SDLW_MakeTexture("assets/log.png", renderer);

    return textures;
}

void SDLW_DestroyTextures(Textures *textures)
{
    SDL_DestroyTexture(textures->playerTexture);
    SDL_DestroyTexture(textures->safeTexture);
    SDL_DestroyTexture(textures->roadTexture);
    SDL_DestroyTexture(textures->waterTexture);
    SDL_DestroyTexture(textures->treeTexture);
    SDL_DestroyTexture(textures->carTexture);
    SDL_DestroyTexture(textures->logTexture);
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

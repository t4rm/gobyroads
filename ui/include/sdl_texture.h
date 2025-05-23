#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

typedef struct texture_t
{
    SDL_Texture *playerTexture;
    SDL_Texture *safeTexture;
    // SDL_Texture *carTexture;
    // SDL_Texture *roadTexture;
    // SDL_Texture *waterTexture;
    // SDL_Texture *treeTexture;
    // SDL_Texture *logTexture;
} Textures;

Textures *initTextures(SDL_Renderer *renderer);
SDL_Texture *makeTextures(char *sprite_name, SDL_Renderer *renderer);
void destroyTextures(Textures *textures);

#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "string.h"

typedef struct _NamedTexture
{
    char *name;
    SDL_Texture *texture;
    int size; // The size of the side of a sprite in the spritesheet.
} NamedTexture;

typedef struct
{
    const char *name;
    const char *path;
    int size; // The size of the side of a sprite in the spritesheet.
} TextureInfo;

typedef struct _TextureCollection
{
    NamedTexture *textures;
    int count;
} TextureCollection;

TextureCollection *SDLW_InitTextures(SDL_Renderer *renderer);
SDL_Texture *GetTexture(TextureCollection *collection, const char *name);
void SDLW_DestroyTextures(TextureCollection *collection);
SDL_Texture *SDLW_MakeTexture(char *sprite_name, SDL_Renderer *renderer);

#endif

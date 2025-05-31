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
} NamedTexture;

typedef struct
{
    const char *name;
    const char *path;
} TextureInfo;

typedef struct _TextureCollection
{
    NamedTexture *textures;
    int count;
} TextureCollection;

typedef struct _CarConfig
{
    int size;
    char variant;
    const char **colors;
    int colorCount;
    const char *staticName; // Nom pour texture statique (NULL si colorée)
    const char *staticPath; // Chemin pour texture statique (NULL si colorée)
} CarConfig;

TextureCollection *SDLW_InitTextures(SDL_Renderer *renderer);
SDL_Texture *GetTexture(TextureCollection *collection, const char *name);
void SDLW_DestroyTextures(TextureCollection *collection);
SDL_Texture *SDLW_MakeTexture(char *sprite_name, SDL_Renderer *renderer);

#endif

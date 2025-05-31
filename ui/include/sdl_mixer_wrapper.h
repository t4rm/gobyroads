#ifndef SDL_AUDIO_WRAPPER_H
#define SDL_AUDIO_WRAPPER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    char *name;
    Mix_Music *music;
} NamedMusic;

typedef struct
{
    char *name;
    Mix_Chunk *chunk;
} NamedChunk;

typedef struct
{
    NamedMusic *musics;
    int musicCount;

    NamedChunk *chunks;
    int chunkCount;
} AudioCollection;

AudioCollection *SDLW_InitAudio();

Mix_Music *GetMusic(AudioCollection *collection, const char *name);
Mix_Chunk *GetChunk(AudioCollection *collection, const char *name);
void SDLW_DestroyAudio(AudioCollection *collection);

#endif

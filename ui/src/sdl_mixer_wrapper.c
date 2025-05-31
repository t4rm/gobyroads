#include "sdl_mixer_wrapper.h"

typedef struct
{
    const char *name;
    const char *path;
} AudioInfo;

static AudioInfo musicData[] = {
    {"bgm", "assets/audio/music/bgm.ogg"},
    {"gameover", "assets/audio/music/gameover.mp3"}};

static AudioInfo chunkData[] = {
    {"splash", "assets/audio/sfx/splash.mp3"},
    {"train", "assets/audio/sfx/train.mp3"},
    {"crash", "assets/audio/sfx/crash.mp3"},
    {"warning", "assets/audio/sfx/warning.mp3"}};

AudioCollection *SDLW_InitAudio()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer init error: %s", Mix_GetError());
        return NULL;
    }

    AudioCollection *collection = malloc(sizeof(AudioCollection));

    collection->musicCount = sizeof(musicData) / sizeof(AudioInfo);
    collection->musics = malloc(sizeof(NamedMusic) * collection->musicCount);

    for (int i = 0; i < collection->musicCount; i++)
    {
        collection->musics[i].name = strdup(musicData[i].name);
        collection->musics[i].music = Mix_LoadMUS(musicData[i].path);

        if (!collection->musics[i].music)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Music load error (%s): %s", musicData[i].path, Mix_GetError());
        }
    }

    collection->chunkCount = sizeof(chunkData) / sizeof(AudioInfo);
    collection->chunks = malloc(sizeof(NamedChunk) * collection->chunkCount);

    for (int i = 0; i < collection->chunkCount; i++)
    {
        collection->chunks[i].name = strdup(chunkData[i].name);
        collection->chunks[i].chunk = Mix_LoadWAV(chunkData[i].path);

        if (!collection->chunks[i].chunk)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Chunk load error (%s): %s", chunkData[i].path, Mix_GetError());
    }

    return collection;
}

Mix_Music *GetMusic(AudioCollection *collection, const char *name)
{
    for (int i = 0; i < collection->musicCount; i++)
        if (strcmp(collection->musics[i].name, name) == 0)
            return collection->musics[i].music;
    return NULL;
}

Mix_Chunk *GetChunk(AudioCollection *collection, const char *name)
{
    for (int i = 0; i < collection->chunkCount; i++)
        if (strcmp(collection->chunks[i].name, name) == 0)
            return collection->chunks[i].chunk;
    return NULL;
}

void SDLW_DestroyAudio(AudioCollection *collection)
{
    for (int i = 0; i < collection->musicCount; i++)
    {
        Mix_FreeMusic(collection->musics[i].music);
        free(collection->musics[i].name);
    }
    free(collection->musics);

    for (int i = 0; i < collection->chunkCount; i++)
    {
        Mix_FreeChunk(collection->chunks[i].chunk);
        free(collection->chunks[i].name);
    }
    free(collection->chunks);

    free(collection);
    Mix_CloseAudio();
}

void SDLW_Mix_HaltAllChannelExcept(int channelToKeep)
{
    int totalChannels = Mix_AllocateChannels(-1);
    for (int i = 0; i < totalChannels; i++)
        if (i != channelToKeep && Mix_Playing(i))
            Mix_HaltChannel(i);
}
#ifndef CORE_WRAPPER_H
#define CORE_WRAPPER_H
#include "SDL2/SDL_events.h"
#include "gamestate.h"

typedef struct _PlayerOffset
{
    int x, y;
} PlayerOffset;

typedef struct _UIGameState
{
    GameState *core;
    PlayerOffset *playerOffset;
    int running, intro;
} UIGameState;

typedef enum {PLAYING, LOST, WAITING} EventListeningMode;

UIGameState *initUIGameState(int h, int l);
void destroyUIGameState(UIGameState *uiGs);
void handleEvents(UIGameState *uiGs, SDL_Event *event, EventListeningMode mode);

#endif

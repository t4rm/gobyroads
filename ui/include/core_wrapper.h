#ifndef CORE_WRAPPER_H
#define CORE_WRAPPER_H
#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <sdl2/SDL_mixer.h>
#include "sdl_mixer_wrapper.h"
#include "gamestate.h"

#define PLAYER_MOVE_COOLDOWN 10

typedef struct _PlayerOffset
{
    int x, y;
} PlayerOffset;

typedef enum
{
    OPTION_NONE,
    OPTION_CORE,
    OPTION_UI,
    OPTION_IA
} MenuOption;

typedef struct _MenuHandler
{
    int selectedOptions[3]; // The options ticked with a #.
    MenuOption highligtedOption;
} MenuHandler;

typedef enum
{
    PLAYING,
    LOST,
    WAITING,
    MENU
} EventListeningMode;

typedef struct _UIGameState
{
    GameState *core;
    PlayerOffset *playerOffset;
    MenuHandler *menuHandler;
    int running, intro, menu;
    bool muteSound;
} UIGameState;

UIGameState *initUIGameState(int h, int l);
void destroyUIGameState(UIGameState *uiGs);
void handleEvents(UIGameState *uiGs, SDL_Event *event, EventListeningMode mode);
void SDLW_UpdateTrain(Grid *grid, AudioCollection *audio);
void SDLW_HandleCollision(GameState *gs, AudioCollection *audio);

#endif

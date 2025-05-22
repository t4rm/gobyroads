#include "core_wrapper.h"

UIGameState *initUIGameState(int h, int l)
{
    UIGameState *uiGs = malloc(sizeof(UIGameState));
    uiGs->core = initGameState(h, l);
    uiGs->playerOffset.x = 0;
    uiGs->playerOffset.y = 0;
    uiGs->running = 1;
    return uiGs;
}

void destroyUIGameState(UIGameState *uiGs)
{
    destroyGameState(uiGs->core);
    free(uiGs);
}

void handleEvents(UIGameState *uiGS, SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        uiGS->running = 0;
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {
        // case SDLK_UP:
        case SDLK_DOWN:
            // distance += 1;
            // offsetY = 2;
            // offsetX += 1;
            // offsetX %= 4;
            break;
        }
        break;
    case SDL_KEYUP:
        // offsetX = 0;
        // offsetY = 0;
        break;
    }
}

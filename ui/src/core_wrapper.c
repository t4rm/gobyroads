#include "core_wrapper.h"

UIGameState *initUIGameState(int h, int l)
{
    UIGameState *uiGs = malloc(sizeof(UIGameState));
    uiGs->core = initGameState(h, l);
    uiGs->playerOffset = (PlayerOffset *)malloc(sizeof(PlayerOffset));
    uiGs->playerOffset->x = 0;
    uiGs->playerOffset->y = 0;
    uiGs->running = 1;
    uiGs->intro = 1;
    return uiGs;
}

void destroyUIGameState(UIGameState *uiGs)
{
    destroyGameState(uiGs->core);
    free(uiGs->playerOffset);
    free(uiGs);
}

void handleEvents(UIGameState *uiGs, SDL_Event *event, EventListeningMode mode)
{
    if (mode == PLAYING)
    {
        int hasMoved = 0;
        int precedentX = uiGs->core->player->x, precedentY = uiGs->core->player->y;

        switch (event->type)
        {
        case SDL_QUIT:
            uiGs->running = 0;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
            case SDLK_UP:
                uiGs->playerOffset->y = 0;
                uiGs->core->player->y++;
                hasMoved = 1;
                break;
            case SDLK_LEFT:
                uiGs->playerOffset->y = 3;
                uiGs->core->player->x--;
                hasMoved = 1;
                break;
            case SDLK_RIGHT:
                uiGs->playerOffset->y = 1;
                uiGs->core->player->x++;
                hasMoved = 1;
                break;
            case SDLK_DOWN:
                uiGs->playerOffset->y = 2;
                uiGs->core->player->y--;
                hasMoved = 1;
                break;
            }

            if (hasMoved)
            {
                uiGs->playerOffset->x += 1;
                uiGs->playerOffset->x %= 4;
                if (uiGs->core->player->y >= 0)
                {
                    uiGs->core->player->mouvementCooldown = uiGs->core->grid->cases[uiGs->core->player->y][uiGs->core->player->x] == ICE ? PLAYER_MOVE_COOLDOWN * 2 : PLAYER_MOVE_COOLDOWN;
                }
                else
                    uiGs->core->player->mouvementCooldown = PLAYER_MOVE_COOLDOWN;
                uiGs->core->player->afk = 0;
            }

            if (uiGs->core->player->x >= 0 && uiGs->core->player->y >= 0)
            {
                if (uiGs->core->grid->cases[uiGs->core->player->y][uiGs->core->player->x] == TREE)
                {
                    uiGs->core->player->x = precedentX;
                    uiGs->core->player->y = precedentY;
                    uiGs->core->player->mouvementCooldown = 1;
                }
            }
            break;
        case SDL_KEYUP:
            uiGs->playerOffset->x = uiGs->playerOffset->x <= 1 ? 1 : 3;
            break;
        }
    }
    else if (mode == LOST)
        switch (event->type)
        {
        // case SDL_QUIT:
        // uiGs->running = 0;
        // break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
            case SDLK_f:
                uiGs->running = 0;
                break;
            case SDLK_r:
                destroyGameState(uiGs->core);
                uiGs->core = initGameState(uiGs->core->grid->height, uiGs->core->grid->length - (2 * uiGs->core->carMaxSize));
                // if (!uiGs->core)
                // {
                //     uiGs->running = 0;
                //     return;
                // }
                // Est-ce utile de vérifier ici
                uiGs->running = 1;
                uiGs->intro = 0;
                break;
            }
        }
    else
        switch (event->type)
        {
        case SDL_QUIT:
            uiGs->running = 0;
            uiGs->intro = 0;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                uiGs->intro = 0;
                break;
            }
            case SDLK_f:
                uiGs->intro = 0;
                uiGs->running = 0;
                break;
            }
        }
}

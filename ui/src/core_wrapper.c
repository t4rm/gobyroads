#include "core_wrapper.h"

UIGameState *initUIGameState(int h, int l)
{
    UIGameState *uiGs = malloc(sizeof(UIGameState));
    uiGs->core = initGameState(h, l, 1);
    uiGs->playerOffset = (PlayerOffset *)malloc(sizeof(PlayerOffset));
    uiGs->playerOffset->x = 0;
    uiGs->playerOffset->y = 0;
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
    int hasMoved = 0;
    int precedentX = uiGS->core->player->x, precedentY = uiGS->core->player->y;

    switch (event->type)
    {
    case SDL_QUIT:
        uiGS->running = 0;
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {
        case SDLK_UP:
            uiGS->playerOffset->y = 0;
            uiGS->core->player->y++;
            uiGS->core->score++;
            hasMoved = 1;
            break;
        case SDLK_LEFT:
            uiGS->playerOffset->y = 3;
            uiGS->core->player->x--;
            hasMoved = 1;
            break;
        case SDLK_RIGHT:
            uiGS->playerOffset->y = 1;
            uiGS->core->player->x++;
            hasMoved = 1;
            break;
        case SDLK_DOWN:
            uiGS->playerOffset->y = 2;
            uiGS->core->player->y--;
            uiGS->core->score--;
            hasMoved = 1;
            break;
        }

        if (hasMoved)
        {
            uiGS->playerOffset->x += 1;
            uiGS->playerOffset->x %= 4;
            uiGS->core->player->mouvementCooldown = 3;
            uiGS->core->player->afk = 0;
        }

        if (uiGS->core->player->x >= 0 && uiGS->core->player->y >= 0)
        {
            if (uiGS->core->grid->cases[uiGS->core->player->y][uiGS->core->player->x] == TREE)
            {
                uiGS->core->player->x = precedentX;
                uiGS->core->player->y = precedentY;
                uiGS->core->player->mouvementCooldown = 1;
            }
        }
        break;
    case SDL_KEYUP:
        uiGS->playerOffset->x = uiGS->playerOffset->x <= 1 ? 1 : 3;
        break;
    }
}

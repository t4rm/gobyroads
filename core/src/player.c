#include <conio.h>
#include "player.h"

void playerMove(GameState *gs)
{
    if (_kbhit())
    {
        char key = _getch();
        bool hasMoved = false;
        int precedentX = gs->player->x, precedentY = gs->player->y;

        playerMoveKey(key, gs, &hasMoved);

        if (gs->grid->cases[gs->player->y][gs->player->x] == TREE)
        {
            gs->player->x = precedentX;
            gs->player->y = precedentY;
            gs->player->mouvementCooldown = 1;
        }

        if (hasMoved)
        {
            gs->player->mouvementCooldown = 8;
            gs->player->afk = 0;
            gs->player->slidingCooldown = 36;
        }
    }

    gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
    gs->player->afk++;
}

void playerMoveKey(char key, GameState *gs, bool *hasMoved)
{

    int precedentX = gs->player->x, precedentY = gs->player->y;

    gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
    gs->player->afk++;

    if (gs->player->mouvementCooldown > 0)
        return;

    switch (key)
    {
    case 'z':
        if (gs->player->y < gs->grid->height - 1)
        {
            gs->player->y++;
            *hasMoved = true;
            gs->player->lastMoove = 1;
        }
        break;
    case 's':
        if (gs->player->y > 0)
        {
            gs->player->y--;
            *hasMoved = true;
            gs->player->lastMoove = -1;
        }
        else
            gs->gameOver = true;
        break;
    case 'q':
        if (gs->player->x > gs->carMaxSize + 1)
        {
            gs->player->x--;
            *hasMoved = true;
            gs->player->lastMoove = 2;
        }
        break;
    case 'd':
        if (gs->player->x < gs->grid->length - gs->carMaxSize)
        {
            gs->player->x++;
            *hasMoved = true;
            gs->player->lastMoove = -2;
        }
        break;
    case 'f':
        gs->gameOver = true;
        break;
    }

    if (gs->grid->cases[gs->player->y][gs->player->x] == TREE)
    {
        gs->player->x = precedentX;
        gs->player->y = precedentY;
        gs->player->mouvementCooldown = 1;
    }

    if (hasMoved)
    {
        gs->player->mouvementCooldown = 8;
        gs->player->afk = 0;
        gs->player->slidingCooldown = 36;
    }
}
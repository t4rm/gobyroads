#include <conio.h>
#include "player.h"

void playerMove(GameState *gs)
{
    if (_kbhit())
    {
        char key = _getch();
        bool hasMoved = false;
        int precedentX = gs->player->x, precedentY = gs->player->y;

        if (gs->player->mouvementCooldown == 0)
        {
            switch (key)
            {
            case 'z':
                if (gs->player->y < gs->grid->height - 1)
                {
                    gs->player->y++;
                    gs->score++;
                    hasMoved = true;
                }
                break;
            case 's':
                if (gs->player->y > 0)
                {
                    gs->player->y--;
                    gs->score--;
                    hasMoved = true;
                }
                else
                {
                    gs->gameOver = true;
                }
                break;
            case 'q':
                if (gs->player->x > gs->carMaxSize + 1)
                {
                    gs->player->x--;
                    hasMoved = true;
                }
                break;
            case 'd':
                if (gs->player->x < gs->grid->length - gs->carMaxSize)
                {
                    gs->player->x++;
                    hasMoved = true;
                }
                break;
            case 'f':
                gs->gameOver = true;
                break;
            }
        }

        if (gs->grid->cases[gs->player->y][gs->player->x] == TREE)
        {
            gs->player->x = precedentX;
            gs->player->y = precedentY;
            gs->player->mouvementCooldown = 1;
        }

        if (hasMoved)
        {
            gs->player->mouvementCooldown = 3;
            gs->player->afk = 0;
        }
    }

    gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
    gs->player->afk++;
}

#include "agent_ai.h"
#include "gameState.h"
#include "a_star.h"
#include <stdio.h>
#include <windows.h>

Node **getPathAI(GameState *gs, int *pathLength)
{

    if (gs->gameOver == false)
    {

        Node **path = aStar(gs, gs->player->x, gs->player->y, gs->grid->length / 2, gs->grid->height - 1, pathLength);

        if (*pathLength == 0)
        {
            printf("Aucun chemin trouvé");
            return NULL;
        }
        else
        {

            return path;
        }
    }
}

void playerMoveAi(GameState *gs, Node *node)
{
    Player *player = gs->player;
    bool moved = false;
    if (player->x > node->x)
    {

        //  pressKey(0x51);
        playerMoveKey('q', gs, &moved);
    }
    else if (player->x < node->x)
    {

        //   pressKey(0x44);
        playerMoveKey('d', gs, &moved);
    }
    else if (player->y < node->y)
    {

        // pressKey(0x5A);
        playerMoveKey('z', gs, &moved);
    }
    else if (player->y > node->y)
    {

        // pressKey(0x53);
        playerMoveKey('s', gs, &moved);
    }
}

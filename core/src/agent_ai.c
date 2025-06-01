#include "agent_ai.h"
#include "gameState.h"
#include "a_star.h"
#include <stdio.h>

/* Get the path given by A* for the AI
 * gs : pointer to the gamestate
 * pathLength : pointer to the length of the path that will be modified
 * return : the path as an ordered list of nodes or NULL if no path found
 */
Node **getPathAI(GameState *gs, int *pathLength)
{
    *pathLength = 0;

    int acc = 0;
    // we start with the end as the middle cell in the last row
    int endX = gs->grid->length / 2;
    int endY = gs->grid->height - 1;

    Node **path = NULL;

    // while we dont find a path we try with i row before but not less than 3
    while (*pathLength == 0 && acc < gs->grid->height - 3)
    {
        int targetY = endY - acc;

        if (targetY >= 0)
        {
            path = aStar(gs, gs->player->x, gs->player->y, endX, targetY, pathLength);
        }

        acc++;
    }

    if (*pathLength > 0)
        return path;

    return NULL;
}

/* Call playerMoveKey function to move towards the node given
 * gs : pointer to the gamestate
 * node : pointer to the node to move toward
 */
void playerMoveAi(GameState *gs, Node *node)
{
    Player *player = gs->player;
    bool moved = false;

    if (player->x > node->x)
        playerMoveKey('q', gs, &moved);
    else if (player->x < node->x)
        playerMoveKey('d', gs, &moved);
    else if (player->y < node->y)
        playerMoveKey('z', gs, &moved);
    else if (player->y > node->y)
        playerMoveKey('s', gs, &moved);
}
#ifndef PLAYER_H
#define PLAYER_H

#include "gamestate.h"

typedef struct _GameState GameState;

typedef struct _Player
{
    int x, y, mouvementCooldown, afk, lastMoove, slidingCooldown;
} Player;

void playerMove(GameState *gs);
void playerMoveKey(char key, GameState *gs, bool *hasMoved);

#endif

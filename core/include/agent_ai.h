#pragma once

#include "gamestate.h"
#include "a_star.h"

Node **getPathAI(GameState *gs, int *routeLength);
void playerMoveAi(GameState *gs, Node *node);
void aiLoop(GameState *gs, int *pathLength, int *cmpt, Node ***path);

#ifndef GAMESTATE_H
#define GAMESTATE_H

// #include "car.h"
#include "map.h"
#include "stdbool.h"

typedef struct _Player
{
    int x, y, mouvementCooldown;
} Player;

typedef struct _GameState
{
    Grid *grid;
    Player *player;
    int score;
    int backwardMovements;
    int nextSafeZone;
    bool gameOver;
} GameState;

GameState *initGameState();

void playerMove(GameState *gs);
void scrolling(GameState *gs);
void updateGameState();
bool handleCollision(GameState *gameState);

#endif

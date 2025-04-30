#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "map.h"
#include "effect_queue.h"
#include "car_queue.h"
#include "car.h"
#include "effect.h"

typedef struct _Player
{
    int x, y, mouvementCooldown, afk;
} Player;

typedef struct _Car
{
  int x, y, size, direction, speed, accumulator;
} Car;

typedef struct _Grid Grid;
typedef struct _EffectQueue EffectQueue;
typedef struct _CarQueue CarQueue;

typedef struct _GameState
{
    Grid *grid;
    Player *player;
    CarQueue *cars;
    EffectQueue *effects;
    int carsAmount;
    int score;
    int backwardMovements;
    int nextSafeZone;
    int carMaxSize;
    bool gameOver;
} GameState;


GameState *initGameState();
void playerMove(GameState *gs);
void scrolling(GameState *gs);
void updateGameState();
bool handleCollision(GameState *gs);
void destroyGameState(GameState * gs);

#endif

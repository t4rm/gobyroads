#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "map.h"
#include "car_queue.h"
#include "car.h"
#include "player.h"

typedef struct _Player Player;
typedef struct _Grid Grid;
typedef struct _EffectQueue EffectQueue;
typedef struct _CarQueue CarQueue;

typedef struct _Car
{
  int x, y, size, direction;
  Occupation type;
} Car;

typedef struct _GameState
{
  Grid *grid;
  Player *player;
  CarQueue *cars;
  int carsAmount, score, backwardMovements, nextSafeZone, carMaxSize, highestLineReached;
  bool gameOver;

} GameState;

typedef enum
{
  HORN,
  PASSING,
  AWAY
} TrainState;

GameState *
initGameState(int h, int l);
void playerMove(GameState *gs);
void scrolling(GameState *gs);
void handleCollision(GameState *gs);
void destroyGameState(GameState *gs);
void updateGameState(GameState *gs);
void handleScore(GameState *gs);
TrainState updateTrain(Grid *grid);
void updateIce(GameState *gs);
#endif

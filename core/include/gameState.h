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
  int carsAmount;
  int score;
  int backwardMovements;
  int nextSafeZone;
  int carMaxSize;
  bool gameOver;
} GameState;

GameState *initGameState(int h, int l);
void playerMove(GameState *gs);
void scrolling(GameState *gs);
void handleCollision(GameState *gs);
void destroyGameState(GameState *gs);
void updateGameState(GameState *gs);

void updateTrain(Grid *grid);
void updateIce(GameState *gs);
#endif

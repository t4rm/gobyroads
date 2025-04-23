#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "map.h"
#include "deque.h"
#include "que.h"
#include "stdbool.h"

typedef struct _Player
{
    int x, y, mouvementCooldown, afk;
} Player;

typedef struct _Car
{
  int x, y, size, direction, speed, accumulator;
} Car;


typedef struct _Grid Grid;
typedef struct _Que Que;
typedef struct _Deque Deque;

typedef struct _GameState
{
    Grid *grid;
    Player *player;
    Deque *cars;
    Que *effects;
    int carsAmount;
    int score;
    int backwardMovements;
    int nextSafeZone;
    int carMaxSize;
    bool gameOver;
} GameState;


GameState *initGameState();
void addCar(GameState *gs, int y, int direction, int availableSize);
void updateCars(GameState *gs);
void playerMove(GameState *gs);
void scrolling(GameState *gs);
void decrementCarsOnY(GameState *gs);
void decrementEffectsOnY(GameState *gs);
void updateGameState();
bool handleCollision(GameState *gs);
void updateEffects(GameState *gs);
void destroyGameState(GameState * gs);

#endif

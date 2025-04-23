#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "map.h"
#include "deque.h"
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
typedef struct _Deque Deque;

typedef struct _GameState
{
    Grid *grid;
    Player *player;
    Deque *cars;
    int carsAmount;
    int score;
    int backwardMovements;
    int nextSafeZone;
    int carMaxSize;
    bool gameOver;
} GameState;


GameState *initGameState();
void addCar(GameState *gs, int y, int direction);
void updateCars(GameState *gs);
void playerMove(GameState *gs);
void scrolling(GameState *gs);
void decrementCarsOnY(GameState *gs);
void updateGameState();
bool handleCollision(GameState *gs);

#endif

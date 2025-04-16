#ifndef CAR_H
#define CAR_H

#include "gamestate.h"

typedef struct _Car
{
  int position_x, position_y, size, direction, speed, accumulator;
} Car;

typedef struct _GameState GameState;

void add_car(GameState *gameState);
void update_cars(GameState *gameState);

#endif

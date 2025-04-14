#ifndef CAR_H
#define CAR_H
#include "gameState.h"

typedef struct _Car
{
    int position_y, position_x;
    int size;
    int direction;
} Car;

void add_car(GameState *gameState);
void update_cars(GameState *gameState);

#endif

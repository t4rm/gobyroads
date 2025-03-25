#ifndef DEF_CAR
#define DEF_CAR

#include "gameState.h"
#include "stdbool.h"

typedef struct _Car {     
    int position_y, position_x;
    int size;
    int direction;
  }Car;

typedef struct _GameState GameState;

void add_car(GameState *gameState);
void update_cars(GameState *gameState);

#endif 

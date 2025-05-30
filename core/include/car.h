#ifndef CAR_H
#define CAR_H

#include "gamestate.h"
#include <stdlib.h>

typedef struct _GameState GameState;

// void determineMaxesValues(int score, Occupation roadType, int *maxSize, int *maxSpeed, int *minSpeed, int *maxCars);
void addCar(GameState *gs, int y, RowManager *rowManager, int availableSize);
void updateCars(GameState *gs);
void decrementCarsOnY(GameState *gs);

#endif

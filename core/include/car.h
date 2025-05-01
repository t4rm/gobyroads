#ifndef CAR_H
#define CAR_H

#include "gamestate.h"

typedef struct _GameState GameState;

void determineMaxesValues(int score, Occupation roadType, int *maxSize, int *maxSpeed, int *minSpeed, int *maxCars);
void addCar(GameState *gs, int y, int direction, int availableSize, Occupation roadType);
void updateCars(GameState *gs);
void decrementCarsOnY(GameState *gs);

#endif

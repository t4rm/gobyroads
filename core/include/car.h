#ifndef CAR_H
#define CAR_H

#include <stdlib.h>
#include "gamestate.h"
#include "map.h"

typedef struct _GameState GameState;

typedef enum
{
    COLOR_BLACK,
    COLOR_BLUE,
    COLOR_BROWN,
    COLOR_GREEN,
    COLOR_MAGENTA,
    COLOR_RED,
    COLOR_WHITE,
    COLOR_YELLOW,
    COLOR_COUNT // c'est le compteur car les enum sont créés par ordre croissant
} CarColor;

typedef struct _Car
{
    int x, y, size, direction;
    CarColor color; // used by sdl
    char variant;   // used by sdl
    Occupation type;
} Car;

// void determineMaxesValues(int score, Occupation roadType, int *maxSize, int *maxSpeed, int *minSpeed, int *maxCars);
void addCar(GameState *gs, int y, RowManager *rowManager, int availableSize);
void updateCars(GameState *gs);
void decrementCarsOnY(GameState *gs);

#endif

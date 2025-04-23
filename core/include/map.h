#ifndef MAP_H
#define MAP_H

#include "gamestate.h"

typedef enum
{
    ROAD,
    CAR,
    SAFE,
    TREE
} Occupation;

typedef struct _Grid
{
    Occupation **cases;
    int height;
    int length;
} Grid;

typedef struct _GameState GameState;
typedef struct _Player Player;

Grid *createGrid(int heigth, int length, int carMaxSize);
Occupation *createRow(int length, Occupation type);
void applyOccupationToRow(Occupation *row, int length, Occupation type);
void displayGrid(Grid *grid, int score, int playerX, int playerY, int carMaxSize);

#endif

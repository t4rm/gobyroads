#ifndef MAP_H
#define MAP_H

typedef enum
{
    ROAD,
    WATER,
    SAFE,
    TREE,
    CAR_LEFT,
    CAR_RIGHT,
    LOG,
    ICE
} Occupation;

typedef struct _Grid
{
    Occupation **cases;
    int height;
    int length;
} Grid;

Grid *createGrid(int heigth, int length, int carMaxSize);
Occupation *createRow(int length, Occupation type);
void applyOccupationToRow(Occupation *row, int length, Occupation type);
void displayGrid(Grid *grid, int score, int playerX, int playerY, int carMaxSize);
void destroyGrid(Grid *g);

#endif

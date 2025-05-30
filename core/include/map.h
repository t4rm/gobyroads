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
    RAIL,
    WARNING,
    TRAIN,
    ICE
} Occupation;

typedef struct _rowManager
{
    int direction;
    int speed;
    int cooldown;
    Occupation type;
} RowManager;

typedef struct _Grid
{
    RowManager **rowManagers;
    Occupation **cases;
    int height;
    int length;
} Grid;

RowManager *createRowManager(int direction, int speed, Occupation type);
Occupation *createRow(int length, Occupation type);
Grid *createGrid(int heigth, int length, int carMaxSize);
void createTrees(Occupation *row, int length);
void applyOccupationToRow(Occupation *row, int length, Occupation type);
void displayGrid(Grid *grid, int score, int playerX, int playerY, int carMaxSize);
void destroyGrid(Grid *g);

#endif

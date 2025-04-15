#ifndef MAP
#define MAP

#include "gameState.h" 


typedef enum 
{
    ROAD,
    USER,
    CAR,
    SAFE
} Occupation;


typedef struct _Grid
{
    Occupation **cases;
    int height;
    int length;
} Grid;



typedef struct _GameState GameState;
typedef struct  _Player Player;



Grid *createGrid(int heigth, int length);
Occupation *createRow(int length);
void displayGrid(Grid *grid, Player *player);

#endif 

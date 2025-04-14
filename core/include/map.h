#ifndef MAP_H
#define MAP_H
#include "gameState.h" 

typedef enum { ROAD, USER, CAR, SAFE } Occupation;
typedef struct _Row { Occupation *cases; struct _Row *next;} Row;

Row * createGrid(int length, int height);
Occupation * createRow(int length);
void displayGrid(Row *row, int length, Player *player);

#endif 
